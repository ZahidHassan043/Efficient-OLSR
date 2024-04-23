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
 
 
 #ifndef OLSR_REPOSITORIES_H
 #define OLSR_REPOSITORIES_H
 
 #include <set>
 #include <vector>
 
 #include "ns3/ipv4-address.h"
 #include "ns3/nstime.h"
//-------------------------------------------------------------s---------------------------------------created
 #include <stdlib.h>
 #include <time.h>
 #include <math.h>
//-------------------------------------------------------------e---------------------------------------------
 namespace ns3 {
 namespace olsr {
 
 
 struct IfaceAssocTuple
 {
   Ipv4Address ifaceAddr;
   Ipv4Address mainAddr;
   Time time;
 };
 
 static inline bool
 operator == (const IfaceAssocTuple &a, const IfaceAssocTuple &b)
 {
   return (a.ifaceAddr == b.ifaceAddr
           && a.mainAddr == b.mainAddr);
 }
 
 static inline std::ostream&
 operator << (std::ostream &os, const IfaceAssocTuple &tuple)
 {
   os << "IfaceAssocTuple(ifaceAddr=" << tuple.ifaceAddr
      << ", mainAddr=" << tuple.mainAddr
      << ", time=" << tuple.time << ")";
   return os;
 }
 
 struct LinkTuple
 {
   Ipv4Address localIfaceAddr;
   Ipv4Address neighborIfaceAddr;
   Time symTime;
   Time asymTime;
   Time time;
 };
 
 static inline bool
 operator == (const LinkTuple &a, const LinkTuple &b)
 {
   return (a.localIfaceAddr == b.localIfaceAddr
           && a.neighborIfaceAddr == b.neighborIfaceAddr);
 }
 
 static inline std::ostream&
 operator << (std::ostream &os, const LinkTuple &tuple)
 {
   os << "LinkTuple(localIfaceAddr=" << tuple.localIfaceAddr
      << ", neighborIfaceAddr=" << tuple.neighborIfaceAddr
      << ", symTime=" << tuple.symTime
      << ", asymTime=" << tuple.asymTime
      << ", expTime=" << tuple.time
      << ")";
   return os;
 }

 
 struct NeighborTuple
 {
   Ipv4Address neighborMainAddr;
   enum Status
   {
     STATUS_NOT_SYM = 0, // "not symmetric"
     STATUS_SYM = 1, // "symmetric"
   } status; 
   uint8_t willingness;
   uint16_t ch_cost;//-----------------------ch cost created
   uint16_t locX, locY; //---------------------------created
 };
 //-------------------------------------------------------------------------s----------------------------------------------

struct DestTuple  // dest tuple
{
  Ipv4Address destAddr;
  uint16_t dtlocX, dtlocY;
};

struct Mpr_Tuple  // mpr tuple
{
  Ipv4Address mt_selector;
  Ipv4Address mt_dest;
  uint16_t mt_locationX;
  uint16_t mt_locationY;
  uint16_t mt_cost;
  uint16_t mt_ch_cost;
  
};

struct CostTuple  // cost tuple
{
  Ipv4Address ct_dest;
  Ipv4Address ct_nextMpr;
  uint16_t ct_cost;
};
//--------------------------------------------------------------------------e----------------------------------------------
 static inline bool
 operator == (const NeighborTuple &a, const NeighborTuple &b)
 {
   return (a.neighborMainAddr == b.neighborMainAddr
           && a.status == b.status
           && a.willingness == b.willingness);
 }
 
 static inline std::ostream&
 operator << (std::ostream &os, const NeighborTuple &tuple)
 {
   os << "NeighborTuple(neighborMainAddr=" << tuple.neighborMainAddr
      << ", status=" << (tuple.status == NeighborTuple::STATUS_SYM ? "SYM" : "NOT_SYM")
      << ", willingness=" << (int) tuple.willingness << ")";
   return os;
 }
 
 struct TwoHopNeighborTuple
 {
   Ipv4Address neighborMainAddr;
   Ipv4Address twoHopNeighborAddr;
   Time expirationTime; // previously called 'time_'
 };
 
 static inline std::ostream&
 operator << (std::ostream &os, const TwoHopNeighborTuple &tuple)
 {
   os << "TwoHopNeighborTuple(neighborMainAddr=" << tuple.neighborMainAddr
      << ", twoHopNeighborAddr=" << tuple.twoHopNeighborAddr
      << ", expirationTime=" << tuple.expirationTime
      << ")";
   return os;
 }
 
 static inline bool
 operator == (const TwoHopNeighborTuple &a, const TwoHopNeighborTuple &b)
 {
   return (a.neighborMainAddr == b.neighborMainAddr
           && a.twoHopNeighborAddr == b.twoHopNeighborAddr);
 }
 
 struct MprSelectorTuple
 {
   Ipv4Address mainAddr;
   Time expirationTime; // previously called 'time_'
 };
 
 static inline bool
 operator == (const MprSelectorTuple &a, const MprSelectorTuple &b)
 {
   return (a.mainAddr == b.mainAddr);
 }
 
 
 // The type "list of interface addresses"
 //typedef std::vector<nsaddr_t> addr_list_t;
 
 struct DuplicateTuple
 {
   Ipv4Address address;
   uint16_t sequenceNumber;
   bool retransmitted;
   std::vector<Ipv4Address> ifaceList;
   Time expirationTime;
 };
 
 static inline bool
 operator == (const DuplicateTuple &a, const DuplicateTuple &b)
 {
   return (a.address == b.address
           && a.sequenceNumber == b.sequenceNumber);
 }
 
 struct TopologyTuple
 {
   Ipv4Address destAddr;
   Ipv4Address lastAddr;
   uint16_t sequenceNumber;
   Time expirationTime;
 };
 
 static inline bool
 operator == (const TopologyTuple &a, const TopologyTuple &b)
 {
   return (a.destAddr == b.destAddr
           && a.lastAddr == b.lastAddr
           && a.sequenceNumber == b.sequenceNumber);
 }
 
 static inline std::ostream&
 operator << (std::ostream &os, const TopologyTuple &tuple)
 {
   os << "TopologyTuple(destAddr=" << tuple.destAddr
      << ", lastAddr=" << tuple.lastAddr
      << ", sequenceNumber=" << (int) tuple.sequenceNumber
      << ", expirationTime=" << tuple.expirationTime
      << ")";
   return os;
 }
 
 struct Association
 {
   Ipv4Address networkAddr; 
   Ipv4Mask netmask;        
 };
 
 static inline bool
 operator == (const Association &a, const Association &b)
 {
   return (a.networkAddr == b.networkAddr
           && a.netmask == b.netmask);
 }
 
 static inline std::ostream&
 operator << (std::ostream &os, const Association &tuple)
 {
   os << "Association(networkAddr=" << tuple.networkAddr
      << ", netmask=" << tuple.netmask
      << ")";
   return os;
 }
 
 struct AssociationTuple
 {
   Ipv4Address gatewayAddr;
   Ipv4Address networkAddr;
   Ipv4Mask netmask;
   Time expirationTime;
 };
 
 static inline bool
 operator == (const AssociationTuple &a, const AssociationTuple &b)
 {
   return (a.gatewayAddr == b.gatewayAddr
           && a.networkAddr == b.networkAddr
           && a.netmask == b.netmask);
 }
 
 static inline std::ostream&
 operator << (std::ostream &os, const AssociationTuple &tuple)
 {
   os << "AssociationTuple(gatewayAddr=" << tuple.gatewayAddr
      << ", networkAddr=" << tuple.networkAddr
      << ", netmask=" << tuple.netmask
      << ", expirationTime=" << tuple.expirationTime
      << ")";
   return os;
 }
 
 
 typedef std::set<Ipv4Address>                   MprSet; 
 typedef std::vector<MprSelectorTuple>           MprSelectorSet; 
 typedef std::vector<LinkTuple>                  LinkSet; 
 typedef std::vector<NeighborTuple>              NeighborSet; 
 typedef std::vector<TwoHopNeighborTuple>        TwoHopNeighborSet; 
 typedef std::vector<TopologyTuple>              TopologySet; 
 typedef std::vector<DuplicateTuple>             DuplicateSet; 
 typedef std::vector<IfaceAssocTuple>            IfaceAssocSet; 
 typedef std::vector<AssociationTuple>           AssociationSet; 
 typedef std::vector<Association>                Associations; 
 typedef std::vector<DestTuple>                  DestSet;//destination Set type.------------------------created--------------------
 typedef std::vector<Mpr_Tuple>                  Mpr_Set;//Mpr_Set type.--------------------------------created--------------------
 typedef std::vector<CostTuple>                  CostSet;//Cost of next mpr Set type.-------------------created--------------------
 
 }
 }  // namespace ns3, olsr
 
 #endif /* OLSR_REPOSITORIES_H */
