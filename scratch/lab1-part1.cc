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
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
 
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  uint32_t nClients = 1;
  uint32_t nPackets = 1;

  CommandLine cmd (__FILE__);
  cmd.AddValue ("nClients", "Number of client nodes", nClients);
  cmd.AddValue ("nPackets", "Number of packets sent by each client", nPackets);
  cmd.Parse (argc, argv);

  if (nClients < 1 || nClients > 5)
    {
      std::cerr << "nClients must be between 1 and 5" << std::endl;
      return 1;
    }

  if (nPackets < 1 || nPackets > 5)
    {
      std::cerr << "nPackets must be between 1 and 5" << std::endl;
      return 1;
    }
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer serverNode;
  serverNode.Create (1);

  NodeContainer clientNodes;
  clientNodes.Create (nClients);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));


  InternetStackHelper stack;
  stack.Install (serverNode);
  stack.Install (clientNodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4Address serverAddress;

  for (uint32_t i = 0; i < nClients; ++i)
  {
    NodeContainer pair;
    pair.Add (clientNodes.Get (i));
    pair.Add (serverNode.Get (0));

    NetDeviceContainer devices = pointToPoint.Install (pair);

    Ipv4InterfaceContainer interfaces = address.Assign (devices);

    if (i == 0)
    {
      serverAddress = interfaces.GetAddress (1);
    }

    address.NewNetwork ();
  }
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  UdpEchoServerHelper echoServer (9);

  echoServer.SetAttribute ("Port", UintegerValue (15));

  ApplicationContainer serverApps =
    echoServer.Install (serverNode.Get (0));

  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (20.0));

  Ptr<UniformRandomVariable> startTime =
   CreateObject<UniformRandomVariable> ();

  startTime->SetAttribute ("Min", DoubleValue (2.0));
  startTime->SetAttribute ("Max", DoubleValue (7.0));

  for (uint32_t i = 0; i < nClients; ++i)
  {
    UdpEchoClientHelper echoClient (serverAddress, 15);

    echoClient.SetAttribute ("MaxPackets", UintegerValue (nPackets));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

    ApplicationContainer clientApps =
      echoClient.Install (clientNodes.Get (i));

    double start = startTime->GetValue ();

    clientApps.Start (Seconds (start));
    clientApps.Stop (Seconds (20.0));
  }

  Simulator::Stop (Seconds (20.0));
  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
