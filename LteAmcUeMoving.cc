/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 Centre Tecnologic de Telecomunicacions de Catalunya (CTTC)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Manuel Requena <manuel.requena@cttc.es>
 */


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/lte-module.h"
#include "ns3/config-store.h"
#include <ns3/buildings-helper.h>
//#include "ns3/gtk-config-store.h"

using namespace ns3;

// position functions insipred by /examples/wireless/wifi-ap.cc
static void
SetPosition (Ptr<Node> node, Vector position)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  mobility->SetPosition (position);
}

static Vector
GetPosition (Ptr<Node> node)
{
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  return mobility->GetPosition ();
}

static void 
ChangePosition (Ptr<Node> node) 
{
  Vector pos = GetPosition (node);
 
  //if (pos.x <= 10.0)
   // {
    //  pos.x = 100.0; // force CQI to 0
   // }
  //else
   // {
    //  pos.x = 2000.0;
   // }
  pos.x = pos.x+3000;
  SetPosition (node, pos);
  
}

int main (int argc, char *argv[])
{	
  CommandLine cmd;
  cmd.Parse (argc, argv);
	
  // to save a template default attribute file run it like this:
  // ./waf --command-template="%s --ns3::ConfigStore::Filename=input-defaults.txt --ns3::ConfigStore::Mode=Save --ns3::ConfigStore::FileFormat=RawText" --run src/lte/examples/lena-first-sim
  //
  // to load a previously created default attribute file
  // ./waf --command-template="%s --ns3::ConfigStore::Filename=input-defaults.txt --ns3::ConfigStore::Mode=Load --ns3::ConfigStore::FileFormat=RawText" --run src/lte/examples/lena-first-sim

  ConfigStore inputConfig;
  inputConfig.ConfigureDefaults ();

  // parse again so you can override default values from the command line
  cmd.Parse (argc, argv);

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::FriisSpectrumPropagationLossModel"));
  // Uncomment to enable logging
  //lteHelper->EnableLogComponents ();

  // Create Nodes: eNodeB and UE
  NodeContainer enbNodes;
  NodeContainer ueNodes;
  enbNodes.Create (1);
  ueNodes.Create (1);

  // Install Mobility Model
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (enbNodes);
  BuildingsHelper::Install (enbNodes);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (ueNodes);
  BuildingsHelper::Install (ueNodes);

  // Create Devices and install them in the Nodes (eNB and UE)
  NetDeviceContainer enbDevs;
  NetDeviceContainer ueDevs;
//   lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler");
  lteHelper->SetSchedulerType ("ns3::PfFfMacScheduler");
  lteHelper->SetSchedulerAttribute ("CqiTimerThreshold", UintegerValue (3));
  enbDevs = lteHelper->InstallEnbDevice (enbNodes);
  ueDevs = lteHelper->InstallUeDevice (ueNodes);
  
  lteHelper->EnableRlcTraces();
  lteHelper->EnableMacTraces();
  lteHelper->EnablePhyTraces();

  // Attach a UE to a eNB
  lteHelper->Attach (ueDevs, enbDevs.Get (0));
  
  Simulator::Schedule (Seconds (0.1), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (0.2), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (0.3), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (0.4), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (0.5), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (0.6), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (0.7), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (0.8), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (0.9), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (1.), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (1.1), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (1.2), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (1.3), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (1.4), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (1.5), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (1.6), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (1.7), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (1.8), &ChangePosition, ueNodes.Get (0));
  Simulator::Schedule (Seconds (1.9), &ChangePosition, ueNodes.Get (0));

  // Activate a data radio bearer
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lteHelper->ActivateDataRadioBearer (ueDevs, bearer);


  Simulator::Stop (Seconds (2));

  Simulator::Run ();

  //GtkConfigStore config;
  //config.ConfigureAttributes ();

  Simulator::Destroy ();
  return 0;
}
