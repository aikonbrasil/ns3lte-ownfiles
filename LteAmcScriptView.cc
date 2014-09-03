/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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
 */

#include "ns3/core-module.h"
#include <ns3/network-module.h>
#include <ns3/mobility-module.h>
#include <ns3/lte-module.h>

NS_LOG_COMPONENT_DEFINE ("LteAmcScriptView");

using namespace ns3;

int
main (int argc, char *argv[])
{
  NS_LOG_UNCOND ("LteAmcScriptView Simulator");

  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();

  //Creamos los Nodos para la eNB y UE (por ahora simples nodos de ns3)
  NodeContainer enbNodes;
  enbNodes.Create (1);
  NodeContainer ueNodes;
  ueNodes.Create(1);

  //Configuramos los parametros de Mobilidad de los Nodos
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (enbNodes);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (ueNodes);

  //Instalamos las funcionalidades de LTe en el nodo de la eNodeB
  NetDeviceContainer enbDevs;
  enbDevs = lteHelper->InstallEnbDevice(enbNodes);
  NetDeviceContainer ueDevs;
  ueDevs = lteHelper->InstallUeDevice (ueNodes);

  //Criamos la conexion RRC
  lteHelper->Attach (ueDevs, enbDevs.Get (0));

  //Activamos el radio Bearer entre a UE e eNB
  enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
  EpsBearer bearer (q);
  lteHelper->ActivateDataRadioBearer (ueDevs, bearer);

  //Activamos las estadísticas
  lteHelper->EnablePhyTraces ();
  lteHelper->EnableMacTraces ();
  lteHelper->EnableRlcTraces ();
  lteHelper->EnablePdcpTraces ();
  lteHelper->EnableUlRxPhyTraces();

  Simulator::Stop (Seconds (0.5));
  Simulator::Run ();

  Simulator::Destroy ();
  return 0;
}
