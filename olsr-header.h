 /* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
 /*
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
  * Author: Gustavo J. A. M. Carneiro  <gjc@inescporto.pt>
  */
 
 #ifndef OLSR_HEADER_H
 #define OLSR_HEADER_H
 
 #include <stdint.h>
 #include <vector>
 #include "ns3/header.h"
 #include "ns3/ipv4-address.h"
 #include "ns3/nstime.h"
 
 
 namespace ns3 {
 namespace olsr {
 
 double EmfToSeconds (uint8_t emf);
 uint8_t SecondsToEmf (double seconds);
 
 class PacketHeader : public Header
 {
 public:
   PacketHeader ();
   virtual ~PacketHeader ();
 
   void SetPacketLength (uint16_t length)
   {
     m_packetLength = length;
   }
 
   uint16_t GetPacketLength () const
   {
     return m_packetLength;
   }
 
   void SetPacketSequenceNumber (uint16_t seqnum)
   {
     m_packetSequenceNumber = seqnum;
   }
 
   uint16_t GetPacketSequenceNumber () const
   {
     return m_packetSequenceNumber;
   }
 
 private:
   uint16_t m_packetLength;          
   uint16_t m_packetSequenceNumber;  
 
 public:
   static TypeId GetTypeId (void);
   virtual TypeId GetInstanceTypeId (void) const;
   virtual void Print (std::ostream &os) const;
   virtual uint32_t GetSerializedSize (void) const;
   virtual void Serialize (Buffer::Iterator start) const;
   virtual uint32_t Deserialize (Buffer::Iterator start);
 };
 
 class MessageHeader : public Header
 {
 public:
   enum MessageType
   {
     HELLO_MESSAGE = 1,
     TC_MESSAGE    = 2,
     MID_MESSAGE   = 3,
     HNA_MESSAGE   = 4,
   };
 
   MessageHeader ();
   virtual ~MessageHeader ();
 
   void SetMessageType (MessageType messageType)
   {
     m_messageType = messageType;
   }
   MessageType GetMessageType () const
   {
     return m_messageType;
   }
 
   void SetVTime (Time time)
   {
     m_vTime = SecondsToEmf (time.GetSeconds ());
   }
   Time GetVTime () const
   {
     return Seconds (EmfToSeconds (m_vTime));
   }
 
   void SetOriginatorAddress (Ipv4Address originatorAddress)
   {
     m_originatorAddress = originatorAddress;
   }
   Ipv4Address GetOriginatorAddress () const
   {
     return m_originatorAddress;
   }
 
   void SetTimeToLive (uint8_t timeToLive)
   {
     m_timeToLive = timeToLive;
   }
   uint8_t GetTimeToLive () const
   {
     return m_timeToLive;
   }
 
   void SetHopCount (uint8_t hopCount)
   {
     m_hopCount = hopCount;
   }
   uint8_t GetHopCount () const
   {
     return m_hopCount;
   }
 
   void SetMessageSequenceNumber (uint16_t messageSequenceNumber)
   {
     m_messageSequenceNumber = messageSequenceNumber;
   }
   uint16_t GetMessageSequenceNumber () const
   {
     return m_messageSequenceNumber;
   }
 
 private:
   MessageType m_messageType;        
   uint8_t m_vTime;                  
   Ipv4Address m_originatorAddress;  
   uint8_t m_timeToLive;             
   uint8_t m_hopCount;               
   uint16_t m_messageSequenceNumber; 
   uint16_t m_messageSize;           
 
 public:
   static TypeId GetTypeId (void);
   virtual TypeId GetInstanceTypeId (void) const;
   virtual void Print (std::ostream &os) const;
   virtual uint32_t GetSerializedSize (void) const;
   virtual void Serialize (Buffer::Iterator start) const;
   virtual uint32_t Deserialize (Buffer::Iterator start);
 
   struct Mid
   {
     std::vector<Ipv4Address> interfaceAddresses;  
 
     void Print (std::ostream &os) const;
     uint32_t GetSerializedSize (void) const;
     void Serialize (Buffer::Iterator start) const;
     uint32_t Deserialize (Buffer::Iterator start, uint32_t messageSize);
   };
 
   struct Hello
   {
     struct LinkMessage
     {
       uint8_t linkCode;       
       std::vector<Ipv4Address> neighborInterfaceAddresses;  
     };
 
     uint8_t hTime;  
//-------------------------------------------------s------------------------------
     uint16_t x,y;//for sender loc
     void SetNodeLoc(uint16_t x1,uint16_t y1){this->x = x1; this->y = y1;}
     uint16_t GetNodeLocX() const {return this->x;}
     uint16_t GetNodeLocY() const {return this->y;}
    
     struct DestMessage  //for senders dest tuple
      {
        Ipv4Address destAddr_hello; 
        uint16_t x_hello;
        uint16_t y_hello;      
      };

     std::vector<DestMessage> destMessages; //!< Dest messages container.
     uint32_t destsSize;//size of dest container.
//-------------------------------------------------e------------------------------
 
     void SetHTime (Time time)
     {
       this->hTime = SecondsToEmf (time.GetSeconds ());
     }
 
     Time GetHTime () const
     {
       return Seconds (EmfToSeconds (this->hTime));
     }
 
     uint8_t willingness; 
     std::vector<LinkMessage> linkMessages; 
 
     void Print (std::ostream &os) const;
     uint32_t GetSerializedSize (void) const;
     void Serialize (Buffer::Iterator start) const;
     uint32_t Deserialize (Buffer::Iterator start, uint32_t messageSize);
   };
//previus tc msg

   struct Tc
   {
     std::vector<Ipv4Address> neighborAddresses; 
     uint16_t ansn,information_tc_size;  
//--------------------------------------------------s----------------------------------------------------
     struct info_tc
     {
     Ipv4Address tcm_mpr, tcm_mprSelector, tcm_dest; 
     uint16_t tcm_cost, tcm_chCost;
     };
     std::vector<info_tc> information_tc;
//--------------------------------------------------e----------------------------------------------------
 
     void Print (std::ostream &os) const;
     uint32_t GetSerializedSize (void) const;
     void Serialize (Buffer::Iterator start) const;
     uint32_t Deserialize (Buffer::Iterator start, uint32_t messageSize);
   };
//--------------------------------------------------s----------------------------------------------------
  /* struct Tc
   {
     struct info_tc
     {
     Ipv4Address tcm_mpr, tcm_mprSelector, tcm_dest; 
     uint16_t tcm_cost, tcm_chCost;
     };
     std::vector<info_tc> information_tc;

     uint16_t ansn,tcm_size;  
 
     void Print (std::ostream &os) const;
     uint32_t GetSerializedSize (void) const;
     void Serialize (Buffer::Iterator start) const;
     uint32_t Deserialize (Buffer::Iterator start, uint32_t messageSize);
   };     */
//--------------------------------------------------e----------------------------------------------------
 
 
   struct Hna
   {
     struct Association
     {
       Ipv4Address address; 
       Ipv4Mask mask;       
     };
 
     std::vector<Association> associations; 
 
     void Print (std::ostream &os) const;
     uint32_t GetSerializedSize (void) const;
     void Serialize (Buffer::Iterator start) const;
     uint32_t Deserialize (Buffer::Iterator start, uint32_t messageSize);
   };
 
 private:
   struct
   {
     Mid mid;      
     Hello hello;  
     Tc tc;        
     Hna hna;      
   } m_message; 
 
 public:
   Mid& GetMid ()
   {
     if (m_messageType == 0)
       {
         m_messageType = MID_MESSAGE;
       }
     else
       {
         NS_ASSERT (m_messageType == MID_MESSAGE);
       }
     return m_message.mid;
   }
 
   Hello& GetHello ()
   {
     if (m_messageType == 0)
       {
         m_messageType = HELLO_MESSAGE;
       }
     else
       {
         NS_ASSERT (m_messageType == HELLO_MESSAGE);
       }
     return m_message.hello;
   }
 
   Tc& GetTc ()
   {
     if (m_messageType == 0)
       {
         m_messageType = TC_MESSAGE;
       }
     else
       {
         NS_ASSERT (m_messageType == TC_MESSAGE);
       }
     return m_message.tc;
   }

 
   Hna& GetHna ()
   {
     if (m_messageType == 0)
       {
         m_messageType = HNA_MESSAGE;
       }
     else
       {
         NS_ASSERT (m_messageType == HNA_MESSAGE);
       }
     return m_message.hna;
   }
 
 
   const Mid& GetMid () const
   {
     NS_ASSERT (m_messageType == MID_MESSAGE);
     return m_message.mid;
   }
 
   const Hello& GetHello () const
   {
     NS_ASSERT (m_messageType == HELLO_MESSAGE);
     return m_message.hello;
   }
 
   const Tc& GetTc () const
   {
     NS_ASSERT (m_messageType == TC_MESSAGE);
     return m_message.tc;
   }

 
   const Hna& GetHna () const
   {
     NS_ASSERT (m_messageType == HNA_MESSAGE);
     return m_message.hna;
   }
 
 
 };
 
 
 static inline std::ostream& operator<< (std::ostream& os, const PacketHeader & packet)
 {
   packet.Print (os);
   return os;
 }
 
 static inline std::ostream& operator<< (std::ostream& os, const MessageHeader & message)
 {
   message.Print (os);
   return os;
 }
 
 typedef std::vector<MessageHeader> MessageList;
 
 static inline std::ostream& operator<< (std::ostream& os, const MessageList & messages)
 {
   os << "[";
   for (std::vector<MessageHeader>::const_iterator messageIter = messages.begin ();
        messageIter != messages.end (); messageIter++)
     {
       messageIter->Print (os);
       if (messageIter + 1 != messages.end ())
         {
           os << ", ";
         }
     }
   os << "]";
   return os;
 }
 
 
 }
 }  // namespace olsr, ns3
 
 #endif /* OLSR_HEADER_H */
