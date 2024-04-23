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
 
 
 #ifndef OLSR_STATE_H
 #define OLSR_STATE_H
 
 #include "olsr-repositories.h"
 
 namespace ns3 {
 namespace olsr {
 
 class OlsrState
 {
   //  friend class Olsr;
 
 protected:
   LinkSet m_linkSet;    
   NeighborSet m_neighborSet;            
   TwoHopNeighborSet m_twoHopNeighborSet;        
   TopologySet m_topologySet;    
   MprSet m_mprSet;      
   MprSelectorSet m_mprSelectorSet;      
   DuplicateSet m_duplicateSet;  
   IfaceAssocSet m_ifaceAssocSet;        
   AssociationSet m_associationSet; 
   Associations m_associations;  
   DestSet m_destSet;// ------------------------------------------created------------------------------------------------------------
   Mpr_Set m_mpr_set;// ------------------------------------------created------------------------------------------------------------
   CostSet m_costSet;// ------------------------------------------created------------------------------------------------------------ 

 public:
   OlsrState ()
   {
   }
 
   // MPR selector
 
   const MprSelectorSet & GetMprSelectors () const
   {
     return m_mprSelectorSet;
   }
 
   MprSelectorTuple* FindMprSelectorTuple (const Ipv4Address &mainAddr);
 
   void EraseMprSelectorTuple (const MprSelectorTuple &tuple);
 
   void EraseMprSelectorTuples (const Ipv4Address &mainAddr);
 
   void InsertMprSelectorTuple (const MprSelectorTuple &tuple);
 
   std::string PrintMprSelectorSet () const;
  
//--------------------------------------------------------------------s-------------------------------created-------
   uint16_t m_LocX, m_LocY;//for node location
   int Weight_cal(float x,float w)
   {//std::cout<<"cost= "<<w<<std::endl;
     return int(x/((2*w)+1));
   }

//--------------------------------------------------------------------d---------------------------------------------
   // Neighbor
 
   const NeighborSet & GetNeighbors () const
   {
     return m_neighborSet;
   }
   NeighborSet & GetNeighbors ()
   {
     return m_neighborSet;
   }
 
   NeighborTuple* FindNeighborTuple (const Ipv4Address &mainAddr);
 
   const NeighborTuple* FindSymNeighborTuple (const Ipv4Address &mainAddr) const;
 
   NeighborTuple* FindNeighborTuple (const Ipv4Address &mainAddr,
                                     uint8_t willingness);
 
   void EraseNeighborTuple (const NeighborTuple &neighborTuple);
   void EraseNeighborTuple (const Ipv4Address &mainAddr);
 
   void InsertNeighborTuple (const NeighborTuple &tuple);
//-----------------------------------------------------------------------s------------------------------------------
//dest table inserttion and finding tuple from it.
 void InsertDestTuple (const DestTuple &destTuple);
 DestTuple* FindDestTuple (const Ipv4Address &dAddr);
 //void EraseDestTuple (DestTuple &destTuple);
 //void EraseDestTuple (Ipv4Address &mainAddr);
 
 const DestSet & GetDests () const
  {
    return m_destSet;
  }
 void ShowDest()
  { //std::cout<<"showing dest tuple "<<std::endl;
  for (DestSet::const_iterator iter = this->m_destSet.begin (); iter != this->m_destSet.end (); iter++)
   {
    std::cout<<iter->destAddr<<", x= "<<iter->dtlocX<<", y= "<<iter->dtlocY<<std::endl;  
   }
  }
 
 uint32_t GetDestSize()//getting size of dest table
  {  
  uint32_t s = 0; 
  for (DestSet::const_iterator iter = this->m_destSet.begin (); iter != this->m_destSet.end (); iter++)
   {
    s += 8; 
   }
  return s;
  }


//mpr_tuple insertion and finding
  void InsertMpr_Tuple (const Mpr_Tuple &mpr_tuple);
  Mpr_Tuple* FindMpr_Tuple (const Ipv4Address &dAddr);
  const Mpr_Set & GetMpr_Set () const
   {
     return m_mpr_set;
   }
  void ShowMpr_Set()
   { //std::cout<<"showing mpr tuple "<<std::endl;
   for (Mpr_Set::const_iterator iter = this->m_mpr_set.begin (); iter != this->m_mpr_set.end (); iter++)
    {
     std::cout<<"Selector: "<<iter->mt_selector<<", dest: "<<iter->mt_dest<<", loc: ("<<iter->mt_locationX<<", "<<iter->mt_locationY<<"), cost: "<<iter->mt_cost<<", ch cost: "<<iter->mt_ch_cost<<std::endl;  
    }
   }
 
  uint32_t GetMpr_Size()//getting size of mpr table
   {  
   uint32_t m = 0; 
   for (Mpr_Set::const_iterator iter = this->m_mpr_set.begin (); iter != this->m_mpr_set.end (); iter++)
    {
     m += 16; 
    }
   return m;
  }

//cost tuple insertion and finding
  void InsertCostTuple (const CostTuple &costTuple);
  CostTuple* FindCostTuple (const Ipv4Address &dAddr);
  const CostSet & GetCostSet () const
   {
     return m_costSet;
   }
  void ShowCostSet()
   { //std::cout<<"showing cost tuple "<<std::endl;
   for (CostSet::const_iterator iter = this->m_costSet.begin (); iter != this->m_costSet.end (); iter++)
    {
     std::cout<<"For dest: "<<iter->ct_dest<<", NextMpr: "<<iter->ct_nextMpr<<", Cost: "<<iter->ct_cost<<std::endl;  
    }
   }
 
  uint32_t GetCostSize()//getting size of cost table
   {  
   uint32_t c = 0; 
   for (CostSet::const_iterator iter = this->m_costSet.begin (); iter != this->m_costSet.end (); iter++)
    {
     c += 10; 
    }
   return c;
   }

  void EraseCostTuple (const CostTuple &tuple);

// A* algorithm
  void aStar();
//-----------------------------------------------------------------------e------------------------------------------
 
   // Two-hop neighbor
 
   const TwoHopNeighborSet & GetTwoHopNeighbors () const
   {
     return m_twoHopNeighborSet;
   }
   TwoHopNeighborSet & GetTwoHopNeighbors ()
   {
     return m_twoHopNeighborSet;
   }
 
   TwoHopNeighborTuple* FindTwoHopNeighborTuple (const Ipv4Address &neighbor,
                                                 const Ipv4Address &twoHopNeighbor);
 
   void EraseTwoHopNeighborTuple (const TwoHopNeighborTuple &tuple);
   void EraseTwoHopNeighborTuples (const Ipv4Address &neighbor);
   void EraseTwoHopNeighborTuples (const Ipv4Address &neighbor,
                                   const Ipv4Address &twoHopNeighbor);
   void InsertTwoHopNeighborTuple (const TwoHopNeighborTuple &tuple);
 
   // MPR
 
   bool FindMprAddress (const Ipv4Address &address);
 
   void SetMprSet (MprSet mprSet);
 
   MprSet GetMprSet () const;
 
   // Duplicate
 
   DuplicateTuple* FindDuplicateTuple (const Ipv4Address &address,
                                       uint16_t sequenceNumber);
 
   void EraseDuplicateTuple (const DuplicateTuple &tuple);
   void InsertDuplicateTuple (const DuplicateTuple &tuple);
 
   // Link
 
   const LinkSet & GetLinks () const
   {
     return m_linkSet;
   }
   LinkTuple* FindLinkTuple (const Ipv4Address &ifaceAddr);
   LinkTuple* FindSymLinkTuple (const Ipv4Address &ifaceAddr, Time time);
   void EraseLinkTuple (const LinkTuple &tuple);
   LinkTuple& InsertLinkTuple (const LinkTuple &tuple);
 
   // Topology
 
   const TopologySet & GetTopologySet () const
   {
     return m_topologySet;
   }
   TopologyTuple* FindTopologyTuple (const Ipv4Address &destAddr,
                                     const Ipv4Address &lastAddr);
   TopologyTuple* FindNewerTopologyTuple (const Ipv4Address &lastAddr,
                                          uint16_t ansn);
   void EraseTopologyTuple (const TopologyTuple &tuple);
   void EraseOlderTopologyTuples (const Ipv4Address &lastAddr,
                                  uint16_t ansn);
   void InsertTopologyTuple (const TopologyTuple &tuple);
 
   // Interface association
 
   const IfaceAssocSet & GetIfaceAssocSet () const
   {
     return m_ifaceAssocSet;
   }
   IfaceAssocSet & GetIfaceAssocSetMutable ()
   {
     return m_ifaceAssocSet;
   }
 
   IfaceAssocTuple* FindIfaceAssocTuple (const Ipv4Address &ifaceAddr);
   const IfaceAssocTuple* FindIfaceAssocTuple (const Ipv4Address &ifaceAddr) const;
   void EraseIfaceAssocTuple (const IfaceAssocTuple &tuple);
   void InsertIfaceAssocTuple (const IfaceAssocTuple &tuple);
 
   // Host-Network Association
   const AssociationSet & GetAssociationSet () const  // Associations known to the node
   {
     return m_associationSet;
   }
 
   const Associations & GetAssociations () const  // Set of associations that the node has
   {
     return m_associations;
   }
 
   AssociationTuple* FindAssociationTuple (const Ipv4Address &gatewayAddr, \
                                           const Ipv4Address &networkAddr, \
                                           const Ipv4Mask &netmask);
   void EraseAssociationTuple (const AssociationTuple &tuple);
   void InsertAssociationTuple (const AssociationTuple &tuple);
   void EraseAssociation (const Association &tuple);
   void InsertAssociation (const Association &tuple);
 
   std::vector<Ipv4Address>
   FindNeighborInterfaces (const Ipv4Address &neighborMainAddr) const;
 
 };
 
 }
 }  // namespace olsr,ns3
 
 #endif /* OLSR_STATE_H */
