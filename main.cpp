#include <iostream>
#include <string>
#include <list>
#include <stdlib.h>
#include <thread>
#include <chrono>

using namespace std;

// Classes 
enum ICMP_t { ADVERTISEMENT, SOLICITATION }; // advertisement is type 9, solicitation is type 10
class ICMP
{
	public:
		// Constructor
		ICMP( ICMP_t t, string i, bool home, bool foreign, bool registration )
			: type(t), IP(i), H(home), F(foreign), R(registration) {}

		// Member Functions
		void insertCOA(string careOfAddress)
		{
			COA.push_front(careOfAddress);
		}

		string getCOA()
		{
			string address = COA.front();
			COA.pop_front();
			return address;
		}

		void printICMP()
		{
			cout << "ICMP: Type(";
			if(type == ADVERTISEMENT) cout << "9/ADVERTISEMENT";
			else cout << "10/SOLICITATION";
			cout << "), IP(" << IP << "), H(" << H << "), F(" << F << "), R(" << R << ")" << endl << endl;
		}

	private:
		// Data members
		ICMP_t type;		// ADVERTISEMENT or SOLICITATION
		string IP;			// IP address
		bool H;				// Home agent bit
		bool F;				// Foreign agent bit
		bool R;				// Registration required bit
		list<string> COA;	// List of available Care-of-Addresses in foreign network
};

enum registration_t { REQUEST, REPLY };
class registrationMessage
{
   public:
	    // Constructor
		registrationMessage( registration_t type, string c, string h, string m, int l, int i )
			: registerType(type), COA(c), HAAddress(h), MNAddress(m), lifeTime(l), id(i) {}

		// Member Functions
		registration_t getRegisterType() { return registerType; }
		string getCOA(){ return COA; }
		string getHAAddress(){ return HAAddress; }
		string getMNAddress(){ return MNAddress; }
		int getLifetime(){ return lifeTime; }
		int getID(){ return id; }

		void printRegistration(bool encapsulation)
		{
			if(encapsulation) cout << "[ENCAPSULATED] ";
			cout << "Registration(";
			if(registerType == REQUEST) cout << "Request): ";
			else cout << "Reply): ";			
			if(COA != "") cout << "COA(" << COA << "), ";
			cout << "HA(" << HAAddress << "), MA(" << MNAddress << "), Lifetime(" << lifeTime << "), ID(" << id << ")";			
			cout << endl << endl;
		}

   private:
	   // Data Members
   	   registration_t registerType; // REQUEST or REPLY
	   string COA;					// Care-of-Address of mobile node in foreign network
	   string HAAddress;			// Home agent address
	   string MNAddress;			// Mobile node permanent address
	   int lifeTime;				// Lifetime of requested registration
	   int id;						// 64-bit ID of message (Acts like sequence number to match REQUEST/REPLY)
};

class mobileNode
{
   public:
      // Constructor
      mobileNode(string internetProtocol, string MACAddress) : IP (internetProtocol), MAC(MACAddress) {}
      
      // Member Functions
      string getIP() { return IP; }
      string getMAC() { return MAC; }
	  void setCOA(string careOfAddress) { COA = careOfAddress; }
	  string getCOA() { return COA; }
      
   private:
      // Members
      string IP;   // This is the permanent IP of the MN's home address 
      string MAC;
	  string COA;  // Care-of-Address/current location of mobile node
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

      void addEntry(string home, string coa, int time)
      {
         // Add new binding entry to Mobility Binding Table
         bindingEntry temp(home, coa, time);
         bindingTable.push_front(temp);
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
		for (unsigned i = 0; i < (15 - IP.length()); i++) cout << " ";
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

		for (int i = 0; i < leftSpace; i++)	cout << " ";
		cout << val;
 		for (int i = 0; i < rightSpace; i++) cout << " ";
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

      void addEntry(string home, string HA, string MAC, int time)
      {
         // Add new binding entry to Mobility Binding Table
         visitorEntry temp(home, HA, MAC, time);
         visitorList.push_front(temp);
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
		for (unsigned i = 0; i < (15 - IP.length()); i++) cout << " ";
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

		for (int i = 0; i < leftSpace; i++)	cout << " ";
		cout << val;
 		for (int i = 0; i < rightSpace; i++) cout << " ";
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
               :source(src), destination(dest), TTL(time), ID(i){}
               
      // Member Functions
      string getSrc() { return source; }
      string getDest() { return destination; }
      int getTTL() { return TTL; }
      void print() {

          cout << "Src: " << source << " Dest: " << destination << " TTL: " << TTL << " Id: " << ID << endl << endl;
	  }
	
	  void decreaseTTL(){TTL--;}

   private:
      // Members
      string source;    // Source address
      string destination;     // Destination address
      int TTL;  // Lifetime of the registration entry
      int ID;        // Identification number of the datagram      
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
void agentDiscovery(mobileNode&, homeAgent, foreignAgent, char);
void registerMN(mobileNode, homeAgent, foreignAgent);
void reverseTunnel(mobileNode, homeAgent, foreignAgent, correspondentNode);
void routeOptimization(mobileNode, correspondentNode);

// Main Simulation
int main()
{
	// Seed time
	srand((unsigned int) time(NULL));

    // Initialize objects
    mobileNode MN(generateIP(), generateMAC());
	homeAgent HA(MN.getIP().replace(MN.getIP().find_last_of("."), 4, "." + to_string(rand() % 254 + 1)));
    foreignAgent FA(generateIP());
    correspondentNode CN(generateIP());
	char selection;
	bool keepRunning = true;

	// Display information
	cout << endl << "----------------------INFORMATION------------------------" << endl << endl;
	cout << "Mobile Node IP: " << MN.getIP() << endl << endl;
	cout << "Home Agent address: " << HA.getHA() << endl;
	HA.printEntries();
	cout << endl;
	cout << "Foreign Agent address: " << FA.getFA() << endl;
	FA.printEntries();
	cout << endl;
	cout << "----------------------INFORMATION------------------------" << endl << endl << endl;

    // Display Main Menu
	do {
		cout << "---------------------------------------------------------" << endl;
		cout << "                        Mobile IP                        " << endl;
	    cout << "---------------------------------------------------------" << endl;
	    cout << "Select if mobile node is in (H)ome or (F)oreign network: ";
	    cin >> selection;
	    switch(selection)
 		{
			case 'H':
			case 'h':
			case 'F':
			case 'f':
				keepRunning = false;
				break;
			default:
				cout << endl << "Incorrect input, try again!";		
		}
		cout << endl;
	} while(keepRunning);

    // Agent Discovery
    agentDiscovery(MN, HA, FA, selection);
   
	// If mobile node is in foreign network
    if(selection == 'F' || selection == 'f')
    {
	    // Register Mobile Node to Home Agent
	    registerMN( MN, HA, FA );   

	    routeOptimization(MN, CN);
    }
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

void agentDiscovery(mobileNode &m, homeAgent h, foreignAgent f, char homeOrForeign)
{
	// Select method (advertisement or solicitation)
	char selection;
	bool keepRunning = true;
	do {
		cout << "---------------------------------------------------------" << endl;
		cout << "                     Agent Discovery                     " << endl;
		cout << "---------------------------------------------------------" << endl;
		cout << "1. Advertisement" << endl;
		cout << "2. Solicitation" << endl;
		cout << "Enter agent discovery method: ";
		cin >> selection;
		switch(selection)
		{
			case '1':
			case '2':
				keepRunning = false;
				break;
			default:
				cout << endl << "Incorrect input, try again!";		
		}
		cout << endl;
	} while(keepRunning);

	// Solicitation
	if(selection == '2')
	{
		// Initialize ICMP solicitation message
		ICMP solicitation(SOLICITATION, m.getIP(), false, false, false);

		// Mobile node broadcast ICMP message to agent in network
		cout << "Mobile Node broadcasting solicitation..." << endl;
		solicitation.printICMP();
		Sleep(2);
	}

	// Advertisement
		// Listen for broadcast
		cout << "Mobile Node listening for Home Agent or Foreign Agent advertisement..." << endl << endl;
		Sleep(2);

		// Agent broadcast ICMP message/advertisement
		// If home network
		if(homeOrForeign == 'H' || homeOrForeign == 'h')
		{
			// Initialize ICMP advertisement message
			ICMP advertisement(ADVERTISEMENT, h.getHA(), true, false, false);
			advertisement.insertCOA(h.getHA());

			// Print advertisement
			if( selection == '2') cout << "Home Agent UNICASTING advertisement... " << endl;
			else cout << "Home Agent BROADCASTING advertisement... " << endl;
			advertisement.printICMP();
		}
		// Otherwise, foreign network
		else
		{
			// Initialize ICMP advertisement message
			ICMP advertisement(ADVERTISEMENT, f.getFA(), false, true, true);
			advertisement.insertCOA(f.getFA());
			
			// Print advertisement
			if( selection == '2') cout << "Foreign Agent UNICASTING advertisement... " << endl;
			else cout << "Foreign Agent BROADCASTING advertisement... " << endl;
			advertisement.printICMP();
		}
		Sleep(2);	
	

	// Check if function is not at home network	
	if( homeOrForeign == 'F' || homeOrForeign == 'f')
    {    
	   // Set COA for mobile node
		m.setCOA(f.getFA());

       // Confirm Mobile Node is in foreign network
       cout << "Mobile Node is in foreign network!" << endl << endl;
       Sleep(2);
	}

	// Otherwise, mobile IP is not needed
    else cout << "Mobile Node is in home network, no need for Mobile IP!" << endl;

	// Print divisor for next section
	cout << "---------------------------------------------------------" << endl << endl;
}

/* steps on page 568 */
void registerMN( mobileNode m, homeAgent h, foreignAgent f )
{
	// Display section title
	cout << "---------------------------------------------------------" << endl;
	cout << "                Registration with Home Agent             " << endl;
	cout << "---------------------------------------------------------" << endl;
         
	// Create registration lifetime and ID
	int lifetimeRequest = rand() % 800 + 199;
	int lifetimeReply = lifetimeRequest - rand() % 199;
	int registrationId = rand() % 999;

    // MN: send request to foreign agent
		// Initialize registration REQUEST
		registrationMessage request(REQUEST, m.getCOA(), h.getHA(), m.getIP(), lifetimeRequest, registrationId);
	    cout << "Mobile Node: Sending registration request to Foreign Agent..." << endl;
		request.printRegistration(false);
		cout << endl;
	    Sleep(2);

    // FA: update visitor list
    cout << "Foreign Agent: Updating Visitor List..." << endl << endl;
    Sleep(2);
    f.addEntry(m.getIP(), h.getHA(), m.getMAC(), lifetimeRequest);
    f.printEntries();
    cout << endl << "Visitor List is updated!" << endl << endl << endl;
    Sleep(3);

    // FA: forward request to home agent
    cout << "Foreign Agent: Forwarding registration request to Home Agent..." << endl;
    request.printRegistration(true);
	cout << endl;
	Sleep(2);
            
    // HA: update binding table
    cout << "Home Agent: Updated Mobile Binding Table..." << endl << endl;
    Sleep(2);
    h.addEntry(m.getIP(), f.getFA(), lifetimeReply);
    h.printEntries();
    cout << endl << "Mobile Binding Table is updated!" << endl << endl << endl;
    Sleep(3);

	// HA: send reply to foreign agent
		// Initialize registration REPLY
		registrationMessage reply(REPLY, "", h.getHA(), m.getIP(), lifetimeReply, registrationId);
		cout << "Home Agent: Sending registration reply to Foreign Agent..." << endl;
		reply.printRegistration(true);
		cout << endl;
		Sleep(2);
            
    // FA: relay reply to mobile node
    cout << "Foreign Agent: Forwarding registration reply to Mobile Node..." << endl;
	reply.printRegistration(false);
    Sleep(2);   

	// Print divisor for next section
	cout << "---------------------------------------------------------" << endl << endl;
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