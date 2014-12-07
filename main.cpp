#include <iostream>
#include <string>
#include <list>
#include <stdlib.h>
#include <thread>
#include <chrono>

using namespace std;

// Classes 
class mobileNode
{
   public:
      // Constructor
      mobileNode(string internetProtocol, string MACAddress) : IP (internetProtocol), MAC(MACAddress) {}
      
      // Member Functions
      string getIP() { return IP; }
      string getMAC() { return MAC; }

      void sendRequest( registrationMessage &msg )
      {
      	
      }

      void receiveReply( registrationMessage &msg )
      {

      }
      
   private:
      // Members
      string IP;   // This is the permanent IP of the MN's home address 
      string MAC;
};

class correspondentNode
{
	public: 
		// Constructor
		correspondentNode(string internetProtocol) {IP = internetProtocol; }

		// Member Functions
		string getIP()  { return IP; }

	private:
		// Members
		string IP;  // IP for the web server, etc.
};

class homeAgent
{
   public:
      // Constructor
      homeAgent(string HA) { HAAddress = HA; }
      
      // Member Functions
      string getHA() { return HAAddress; }

      bool isEmpty() { return bindingTable.empty(); }

      void receiveRequest( registrationMessage &msg )
      {
		//addEntry()      	
      }

      void addEntry(string home, string coa, int time)
      {
         // Add new binding entry to Mobility Binding Table
         bindingEntry temp(home, coa, time);
         bindingTable.push_front(temp);
      }

      void sendReply( registrationMessage &msg )
      {
      	
      }

      void printEntries()
      {
         // Print Binding Table title
         cout << "-----------------------------------------------------";
         cout << endl;
         cout << "               Mobility Binding Table                ";
         cout << endl;
         cout << "-----------------------------------------------------";
         cout << endl;
         cout << "|  Home Address   | Care-of-Address | Lifetime(sec) |";
         cout << endl;
         cout << "-----------------------------------------------------";
         cout << endl;
                  
         // Iterate through Mobility Binding Table and print each entry
         std::list<bindingEntry>::const_iterator iterator;
         for(iterator = bindingTable.begin(); 
                              iterator != bindingTable.end(); ++iterator)
         {
             cout << "| " << (*iterator).homeAddress;
             printSpaceAndBar((*iterator).homeAddress);
             cout << (*iterator).COA;
             printSpaceAndBar((*iterator).COA);
             printLifeTime((*iterator).lifetime);
             cout << " |" << endl;             
         }
      }
      
   private:
      // Mobility Binding Table entries
      class bindingEntry
      {
         public:
            // Constructor
            bindingEntry(string home, string careOfAddress, int time)
                  :homeAddress(home), COA(careOfAddress), lifetime(time) {}
          
            // Members
            string homeAddress;  // Home address of a mobility node
            string COA;          // Care-of-Address of a mobility node
            int lifetime;        // Lifetime of the entry in seconds
      };

      // Member Functions
      void printSpaceAndBar(string IP)
      {
	 for (unsigned i = 0; i < (15 - IP.length()); i++)
		cout << " ";
	 cout << " | ";
      }
       
      void printLifeTime(int val)
      {
	 int leftSpace = 6;
	 int rightSpace = 6;
	 int v = val / 10;
	 while (v > 0)
	 {
		if (leftSpace == rightSpace)
			leftSpace--;
		else
			rightSpace--;

		v /= 10;
 	 }

	 for (int i = 0; i < leftSpace; i++)
	 	cout << " ";
	 cout << val;
 	 for (int i = 0; i < rightSpace; i++)
		cout << " ";
	}

      // Data Members
      string HAAddress;                // Home Agent address
      list<bindingEntry> bindingTable; // Mobility Binding Table     
};

class foreignAgent
{
   public:
      // Constructor
      foreignAgent(string FA) { FAAddress = FA; }
         
      // Member Functions
      string getFA() { return FAAddress; }         

      void receiveRequest( registrationMessage &msg )
      {
		// add entry      	
      }
         
      void addEntry(string home, string HA, string MAC, int time)
      {
         // Add new binding entry to Mobility Binding Table
         visitorEntry temp(home, HA, MAC, time);
         visitorList.push_front(temp);
      }

      void sendRequest( registrationMessage &msg )
      {
      	
      }

      void receiveAndSendReply( registrationMessage &msg )
      {
      	
      }

      void printEntries()
      {
         // Print Binding Table title
         cout << "-------------------------------------------------------------------------";
         cout << endl;
         cout << "                              Visitor List                               ";
         cout << endl;
         cout << "-------------------------------------------------------------------------";
         cout << endl;
         cout << "|  Home Address   |   Home Agent    |   Media Address   | Lifetime(sec) |";
         cout << endl;
         cout << "|                 |     Address     |                   |               |";
         cout << endl;
         cout << "-------------------------------------------------------------------------";
         cout << endl;
                  
         // Iterate through Mobility Binding Table and print each entry
         std::list<visitorEntry>::const_iterator iterator;
         for(iterator = visitorList.begin(); 
                              iterator != visitorList.end(); ++iterator)
         {
             cout << "| " << (*iterator).homeAddress;
             printSpaceAndBar((*iterator).homeAddress);
             cout << (*iterator).HAAddress;
             printSpaceAndBar((*iterator).HAAddress);
             cout << (*iterator).mediaAddress << " | ";
             printLifeTime((*iterator).lifetime);
             cout << " |" << endl;
         }
      }
               
   private:
      // Visitor List entries
      class visitorEntry
      {
         public:
            // Constructor
            visitorEntry(string home, string HA, string MAC, int time)
                  :homeAddress(home), HAAddress(HA), mediaAddress(MAC),
                                                          lifetime(time){}
                                                          
            // Members
            string homeAddress;  // Home address of a mobility node
            string HAAddress;    // Home agent address
            string mediaAddress; // MAC address
            int lifetime;        // Lifetime of the entry in seconds
      };
      
      // Member Functions
      void printSpaceAndBar(string IP)
      {
	 for (unsigned i = 0; i < (15 - IP.length()); i++)
		cout << " ";
	 cout << " | ";
      }
      
      void printLifeTime(int val)
      {
	 int leftSpace = 6;
	 int rightSpace = 6;
	 int v = val / 10;
	 while (v > 0)
	 {
		if (leftSpace == rightSpace)
			leftSpace--;
		else
			rightSpace--;

		v /= 10;
 	 }

	 for (int i = 0; i < leftSpace; i++)
	 	cout << " ";
	 cout << val;
 	 for (int i = 0; i < rightSpace; i++)
		cout << " ";
	}
      
      // Data Members
      string FAAddress;               // Foreign Agent address
      list<visitorEntry> visitorList; // Visitor List           
};

class datagram
{
   public:
      // Constructor
      datagram(string src, string dest, int time, int i)
               :source(src), destination(dest), lifetime(time), ID(i){}
               
      // Member Functions
      string getSrc() { return source; }
      string getDest() { return destination; }
      int getLifetime() { return lifetime; }
      void print() {

          cout << "Src: " << source << " Dest: " << destination << " TTL: " << lifetime << " Id: " << ID << endl << endl;
	  }
	
	  void decreaseTTL(){lifetime--;}

   private:
      // Members
      string source;    // Source address
      string destination;     // Destination address
      int lifetime;  // Lifetime of the registration entry
      int ID;        // Identification number of the datagram      
};

class registrationMessage
{
   public:
	registerData( string type, string c, string h, string m, string e, int l, int i )
		: registerType(type), COA(c), HAAddress(h), MNAddress(m), EncapFormat(e), lifeTime(l), id(i){}

	string getRegisterType() { return registerType };
	string getCOA(){ return COA };
	string getHAAddress(){ return HAAddress };
	string getMNAddress(){ return MNAddress };
	string getEncapFormat(){ return EncapFormat };
	int getLifetime(){ return lifeTime };
	int getID(){ return id };

   private:
   	   string registerType; // REQUEST or REPLY
	   string COA;
	   string HAAddress;
	   string MNAddress;
	   string EncapFormat; // encapsulation
	   int lifeTime
	   int id;
};

/*
class applicationLayer
{
   public:
      string message;

};

class transportLayer
{
   // public
   class UDPsegment
   {
      
   }
};

class networkLayer
{
   
};
*/

// Function Prototype Declarations
void Sleep(int);
string generateIP();
string generateMAC();
void registerMN(mobileNode, homeAgent, foreignAgent);
void reverseTunnel(mobileNode, homeAgent, foreignAgent, correspondentNode);
void routeOptimization(mobileNode, correspondentNode);

// Main Simulation
int main()
{
   srand((unsigned int) time(NULL));
   
   // Initialize objects
   mobileNode MN(generateIP(), generateMAC());
   homeAgent HA(generateIP());
   foreignAgent FA(generateIP());
   correspondentNode CN(generateIP());
   
cout << "Mobile Node IP: " << MN.getIP() << endl;
cout << "Home Agent address: " << HA.getHA() << endl;
HA.printEntries();
cout << endl;
cout << "Foreign Agent address: " << FA.getFA() << endl;
FA.printEntries();
cout << endl;

   // Display Main Menu
   cout << "            Mobile IP            " << endl;
   cout << "---------------------------------" << endl;
/*
   cout << "1. Mobile Node in Home Network" << endl;
   cout << "2. Mobile Node in Foreign Network" << endl;
   cout << "Enter your selection: ";
   cin >> selection;
*/
  
   
   // Register Mobile Node to Host Agent
   registerMN( MN, HA, FA );   

   routeOptimization(MN, CN);
   
   return 0;
}

// Function Implementation
void Sleep(int time) { this_thread::sleep_for(chrono::seconds(time)); }

string generateIP()
{
   // Generate random IP address
   int ipInt[4] = { rand() % 31 + 192,
      rand() % 254 + 1,
      rand() % 254 + 1,
      rand() % 254 + 1};
   string IP = to_string(ipInt[0]);   
   for(int i = 1; i < 4; i++) IP += "." + to_string(ipInt[i]);
   
   // return the IP
   return IP;
}

string generateMAC()
{
   // Generate random MAC address
   char buffer[33];
   string MAC = "";

   _itoa_s((rand() % 254 + 1), buffer, 4, 16);
   if (string(buffer).length() == 1)
	MAC += "0";
   MAC += buffer;

   for (int i = 1; i < 6; i++)
   {
   	_itoa_s((rand() % 254 + 1), buffer, 4, 16);
	MAC += "-";
	if (string(buffer).length() == 1)
		MAC += "0";
	MAC += buffer;
   }
  
   return MAC;
}

/* steps on page 568 */
void registerMN( mobileNode m, homeAgent h, foreignAgent f )
{
   // Initialize variables
   registrationMessage regData;

//??
   // Listen for broadcast???
   cout << "Mobile Node listening for Home Agent or Foreign Agent advertisement..." << endl << endl;
   Sleep(2);
   
   // Check if function is not at home network
   if( h.isEmpty() )
   {    
      // Confirm Mobile Node is in foreign network
      cout << "Mobile Node is in foreign network!" << endl << endl;
      Sleep(2);
      
      // MN: send request to foreign agent
      m.sendRequest( regData );
      cout << "Mobile Node: Sending registration request to Foreign Agent..." << endl << endl;
      Sleep(2);

      // FA: relay request to host agent
      f.receiveRequest( regData );
      cout << "Foreign Agent: Sending registration request to Host Agent..." << endl << endl;
      Sleep(2);
      
      // FA: update visitor list
      f.sendRequest( regData );
      cout << "Foreign Agent: Updating Visitor List..." << endl << endl;
      Sleep(2);
      f.addEntry(m.getIP(), h.getHA(), m.getMAC(), 20);
      f.printEntries();
      cout << endl << "Visitor List is updated!" << endl << endl;
      Sleep(3);
      
      // HA: send reply to foreign agent
      cout << "Home Agent: Sending registration reply to Foreign Agent..." << endl << endl;
      Sleep(2);
      
      // HA: update binding table
      cout << "Home Agent: Updated Mobile Binding Table..." << endl << endl;
      Sleep(2);
      h.addEntry(m.getIP(), f.getFA(), 30);
      h.printEntries();
      cout << endl << "Mobile Binding Table is updated!" << endl << endl;
      Sleep(3);
      
      // FA: relay reply to mobile node
      cout << "Foreign Agent: Sending registration reply to Mobile Node..." << endl << endl;
      Sleep(2);
   }
   
   // Otherwise, mobile IP is not needed
   else cout << "Mobile Node is in home network!" << endl;

}


void reverseTunnel(mobileNode MN,homeAgent HA, foreignAgent FA,correspondentNode CN){

	// Send datagram from Mobile node to correspondent node
	cout << "Sending packet from Mobile Node  to Foriegn Agent" << endl << endl;

	datagram originalDatagram(MN.getIP(), FA.getFA(), 15, rand() % 65536);

	// print and update datagram
	originalDatagram.print();
	originalDatagram.decreaseTTL();

	// Encapsulate orignal datagram and send to Home Agent
	cout << "Foreign Agent recieved packet, sending encapsulated packet to Home Agent" << endl << endl;

	datagram encapsDatagram(FA.getFA(), HA.getHA(), 15, rand() % 65536);

	// print information
	encapsDatagram.print();

	// send from Home Agent to Correspondent Node
	cout << "Home Agent recieved packet, sending original packet to Correspondent Node" << endl << endl;

	// print information
	originalDatagram.print();

	// packet arrived
	cout << "Correspondent Node recieved packet" << endl << endl;

}


void routeOptimization(mobileNode MN, correspondentNode CN)
{

	// send Datagram directly to Correspondent Node using permanent home address

	cout << "Mobile Node sending datagram to Correspondent Node using permanent home address" << endl << endl;

	datagram newDatagram(MN.getIP(), CN.getIP(), 15, rand() % 65536);

	newDatagram.print();
}
