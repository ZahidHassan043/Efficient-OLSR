/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
 /*
  * Copyright (c) 2004 Francisco J. Ros
  * Copyright (c) 2007 INESC Porto
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
  * Authors: Francisco J. Ros  <fjrm@dif.um.es>
  *          Gustavo J. A. M. Carneiro <gjc@inescporto.pt>
  */
 
 #ifndef OLSR_AGENT_IMPL_H
 #define OLSR_AGENT_IMPL_H
 
 #include "olsr-header.h"
 #include "ns3/test.h"
 #include "olsr-state.h"
 #include "olsr-repositories.h"
 
 #include "ns3/object.h"
 #include "ns3/packet.h"
 #include "ns3/node.h"
 #include "ns3/socket.h"
 #include "ns3/event-garbage-collector.h"
 #include "ns3/random-variable-stream.h"
 #include "ns3/timer.h"
 #include "ns3/traced-callback.h"
 #include "ns3/ipv4.h"
 #include "ns3/ipv4-routing-protocol.h"
 #include "ns3/ipv4-static-routing.h"
 //----------------------------------------------------------------------s------------------------------------------------------------

//----------------------------------------------------------------------e------------------------------------------------------------
 #include <vector>
 #include <map>
//---------------------------------------------------created by zahid-------------------------start-----------------------------------------------
#include "ns3/mobility-model.h"
//--------------------------------------------------------------------------------------------end-------------------------------------------------
 
 class OlsrMprTestCase;
 
 namespace ns3 {
 namespace olsr {
 
 
 struct RoutingTableEntry
 {
   Ipv4Address destAddr; 
   Ipv4Address nextAddr; 
   uint32_t interface; 
   uint32_t distance; 
 
   RoutingTableEntry () : // default values
     destAddr (), nextAddr (),
     interface (0), distance (0)
   {
   }
 };
 
 class RoutingProtocol;
 
 class RoutingProtocol : public Ipv4RoutingProtocol
 {
 public:
   friend class ::OlsrMprTestCase;

   static TypeId GetTypeId (void);
 
   RoutingProtocol ();
   virtual ~RoutingProtocol ();
 
   void SetMainInterface (uint32_t interface);
 
   void Dump (void);
 
   std::vector<RoutingTableEntry> GetRoutingTableEntries () const;
 
   int64_t AssignStreams (int64_t stream);
 
   typedef void (* PacketTxRxTracedCallback)(const PacketHeader & header, const MessageList & messages);
 
   typedef void (* TableChangeTracedCallback) (uint32_t size);
 
 private:
   std::set<uint32_t> m_interfaceExclusions; 
   Ptr<Ipv4StaticRouting> m_routingTableAssociation; 
 
 public:
   std::set<uint32_t> GetInterfaceExclusions () const
   {
     return m_interfaceExclusions;
   }
 
   void SetInterfaceExclusions (std::set<uint32_t> exceptions);
 
   void AddHostNetworkAssociation (Ipv4Address networkAddr, Ipv4Mask netmask);
 
   void RemoveHostNetworkAssociation (Ipv4Address networkAddr, Ipv4Mask netmask);
 
   void SetRoutingTableAssociation (Ptr<Ipv4StaticRouting> routingTable);
 
   Ptr<const Ipv4StaticRouting> GetRoutingTableAssociation () const;
 
 protected:
   virtual void DoInitialize (void);
 private:
   std::map<Ipv4Address, RoutingTableEntry> m_table; 
 
   Ptr<Ipv4StaticRouting> m_hnaRoutingTable; 
 
   EventGarbageCollector m_events; 
 
   uint16_t m_packetSequenceNumber;    
   uint16_t m_messageSequenceNumber;   
   uint16_t m_ansn;  
 
   Time m_helloInterval;   
   Time m_tcInterval;     
   Time m_midInterval;     
   Time m_hnaInterval;     
   uint8_t m_willingness;  
 
   OlsrState m_state;  
   Ptr<Ipv4> m_ipv4;   
 
   void Clear ();
 
   uint32_t GetSize () const
   {
     return m_table.size ();
   }
 
   void RemoveEntry (const Ipv4Address &dest);
   void AddEntry (const Ipv4Address &dest,
                  const Ipv4Address &next,
                  uint32_t interface,
                  uint32_t distance);
   void AddEntry (const Ipv4Address &dest,
                  const Ipv4Address &next,
                  const Ipv4Address &interfaceAddress,
                  uint32_t distance);
 
   bool Lookup (const Ipv4Address &dest,
                RoutingTableEntry &outEntry) const;
 
   bool FindSendEntry (const RoutingTableEntry &entry,
                       RoutingTableEntry &outEntry) const;
 
   // From Ipv4RoutingProtocol
   virtual Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p,
                                       const Ipv4Header &header,
                                       Ptr<NetDevice> oif,
                                       Socket::SocketErrno &sockerr);
   virtual bool RouteInput (Ptr<const Packet> p,
                            const Ipv4Header &header,
                            Ptr<const NetDevice> idev,
                            UnicastForwardCallback ucb,
                            MulticastForwardCallback mcb,
                            LocalDeliverCallback lcb,
                            ErrorCallback ecb);
   virtual void NotifyInterfaceUp (uint32_t interface);
   virtual void NotifyInterfaceDown (uint32_t interface);
   virtual void NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address);
   virtual void NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address);
   virtual void SetIpv4 (Ptr<Ipv4> ipv4);
   virtual void PrintRoutingTable (Ptr<OutputStreamWrapper> stream, Time::Unit unit = Time::S) const;
 
   void DoDispose ();
 
   void SendPacket (Ptr<Packet> packet, const MessageList &containedMessages);
 
   inline uint16_t GetPacketSequenceNumber ();
 
   inline uint16_t GetMessageSequenceNumber ();
 
   void RecvOlsr (Ptr<Socket> socket);
 
   void MprComputation ();
 
   void RoutingTableComputation ();
 
   Ipv4Address GetMainAddress (Ipv4Address iface_addr) const;
 
   bool UsesNonOlsrOutgoingInterface (const Ipv4RoutingTableEntry &route);
 
   // Timer handlers
   Timer m_helloTimer; 
 
   void HelloTimerExpire ();
 
   Timer m_tcTimer; 
 
   void TcTimerExpire ();
 
   Timer m_midTimer; 
 
   void MidTimerExpire ();
 
   Timer m_hnaTimer; 
 
   void HnaTimerExpire ();
 
   void DupTupleTimerExpire (Ipv4Address address, uint16_t sequenceNumber);
 
   bool m_linkTupleTimerFirstTime; 
 
   void LinkTupleTimerExpire (Ipv4Address neighborIfaceAddr);
 
   void Nb2hopTupleTimerExpire (Ipv4Address neighborMainAddr, Ipv4Address twoHopNeighborAddr);
 
   void MprSelTupleTimerExpire (Ipv4Address mainAddr);
 
   void TopologyTupleTimerExpire (Ipv4Address destAddr, Ipv4Address lastAddr);
 
   void IfaceAssocTupleTimerExpire (Ipv4Address ifaceAddr);
 
   void AssociationTupleTimerExpire (Ipv4Address gatewayAddr, Ipv4Address networkAddr, Ipv4Mask netmask);
 
   void IncrementAnsn ();
 
   olsr::MessageList m_queuedMessages;
   Timer m_queuedMessagesTimer; 
 
   void ForwardDefault (olsr::MessageHeader olsrMessage,
                        DuplicateTuple *duplicated,
                        const Ipv4Address &localIface,
                        const Ipv4Address &senderAddress);
//----------------------------------------------------------------------------------s---------------------------------------------
  void PopulateDestSet (const olsr::MessageHeader &msg, const olsr::MessageHeader::Hello &hello);
  void AddDestTuple (const DestTuple &tuple);
  void ShowDests ();
  void PopulateMpr_SetOfSrc();
  void PopulateMpr_Set();
  void PopulateMpr_Set (const olsr::MessageHeader &msg,const olsr::MessageHeader::Tc &tc);
//----------------------------------------------------------------------------------e---------------------------------------------
 
   void QueueMessage (const olsr::MessageHeader &message, Time delay);
 
   void SendQueuedMessages ();
 
   void SendHello ();
 
   void SendTc ();
 
   void SendMid ();
 
   void SendHna ();
 
   void NeighborLoss (const LinkTuple &tuple);
 
   void AddDuplicateTuple (const DuplicateTuple &tuple);
 
   void RemoveDuplicateTuple (const DuplicateTuple &tuple);
 
   void LinkTupleAdded (const LinkTuple &tuple, uint8_t willingness);
 
   void RemoveLinkTuple (const LinkTuple &tuple);
 
   void LinkTupleUpdated (const LinkTuple &tuple, uint8_t willingness);
 
   void AddNeighborTuple (const NeighborTuple &tuple);
 
   void RemoveNeighborTuple (const NeighborTuple &tuple);
 
   void AddTwoHopNeighborTuple (const TwoHopNeighborTuple &tuple);
 
   void RemoveTwoHopNeighborTuple (const TwoHopNeighborTuple &tuple);
 
   void AddMprSelectorTuple (const MprSelectorTuple  &tuple);
 
   void RemoveMprSelectorTuple (const MprSelectorTuple &tuple);
 
   void AddTopologyTuple (const TopologyTuple &tuple);
 
   void RemoveTopologyTuple (const TopologyTuple &tuple);
 
   void AddIfaceAssocTuple (const IfaceAssocTuple &tuple);
 
   void RemoveIfaceAssocTuple (const IfaceAssocTuple &tuple);
 
   void AddAssociationTuple (const AssociationTuple &tuple);
 
   void RemoveAssociationTuple (const AssociationTuple &tuple);
 
   void ProcessHello (const olsr::MessageHeader &msg,
                      const Ipv4Address &receiverIface,
                      const Ipv4Address &senderIface);
 
   void ProcessTc (const olsr::MessageHeader &msg,
                   const Ipv4Address &senderIface);
 
   void ProcessMid (const olsr::MessageHeader &msg,
                    const Ipv4Address &senderIface);
 
   void ProcessHna (const olsr::MessageHeader &msg,
                    const Ipv4Address &senderIface);
 
   void LinkSensing (const olsr::MessageHeader &msg,
                     const olsr::MessageHeader::Hello &hello,
                     const Ipv4Address &receiverIface,
                     const Ipv4Address &senderIface);
 
   void PopulateNeighborSet (const olsr::MessageHeader &msg,
                             const olsr::MessageHeader::Hello &hello);
 
   void PopulateTwoHopNeighborSet (const olsr::MessageHeader &msg,
                                   const olsr::MessageHeader::Hello &hello);
 
   void PopulateMprSelectorSet (const olsr::MessageHeader &msg,
                                const olsr::MessageHeader::Hello &hello);
 
   int Degree (NeighborTuple const &tuple);
 
   bool IsMyOwnAddress (const Ipv4Address & a) const;
 
   Ipv4Address m_mainAddress; 
 
   // One socket per interface, each bound to that interface's address
   // (reason: for OLSR Link Sensing we need to know on which interface
   // HELLO messages arrive)
   std::map< Ptr<Socket>, Ipv4InterfaceAddress > m_sendSockets; 
   Ptr<Socket> m_recvSocket; 
 
 
   TracedCallback <const PacketHeader &, const MessageList &> m_rxPacketTrace;
 
   TracedCallback <const PacketHeader &, const MessageList &> m_txPacketTrace;
 
   TracedCallback <uint32_t> m_routingTableChanged;
 
   Ptr<UniformRandomVariable> m_uniformRandomVariable;
 
 };
 
 }
 }  // namespace ns3
 
 #endif /* OLSR_AGENT_IMPL_H */
