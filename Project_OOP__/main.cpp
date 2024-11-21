#include "HotelBookingSystem.h"
#include <iostream>
#include <locale.h>

void saveClientsToFile(const BookingRequest clients[], int size, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (int i = 0; i < size; i++) {
            outFile << clients[i].toFileFormat() << endl;
        }
        outFile.close();
        cout << "\n Clients saved to " << filename << "\n" << endl;
    }
    else {
        cerr << "Unable to open file " << filename << endl;
    }
}

//5.7
// ������� ��� ������ � ���������� ��'���� Service � �����
void ServiceFileOperations(Service& service, const string& filename) {
    try {
        ofstream outFile(filename, ios::app); // ³������� � ����� ���������
        if (!outFile) throw runtime_error("Unable to open file for writing");

        // ����� ��'���� � ����
        service.Write(outFile);
        outFile.close();

        ifstream inFile(filename); // ³������� ��� �������
        if (!inFile) throw runtime_error("Unable to open file for reading");

        // ��������� ������ ��'���� ��� ���������� �����
        Service serviceCopy;
        serviceCopy.Read(inFile);
        inFile.close();

        // ������������ �������� ���������� �����
        cout << endl;
        cout << "_________________________" << endl;
        cout << "Service read from file: " << endl;
        cout << "Service Name: " << serviceCopy.getServiceName() << endl;
        cout << "Price: $" << serviceCopy.getPrice() << endl;
    }
    catch (const ios_base::failure& exception) {
        cerr << "I/O error: " << exception.what() << endl;
    }
    catch (const runtime_error& exception) {
        cerr << "Runtime error: " << exception.what() << endl;
    }
    catch (const bad_alloc& exception) {
        cerr << "Memory allocation error: " << exception.what() << endl;
    }
    catch (const exception& exception) {
        cerr << "General error: " << exception.what() << endl;
    }
}


// ��������� � ���������� ������ �� ������ �������
void Service_Dynamic(Service*& services, int& size, double priceLimit) {
    try {
        if (size == 0) {
            // ��������� ����� ��������. ���� �������� ��� ���������
            throw runtime_error("Dynamic array is empty. No items to delete.");
        }

        int indexToDelete = -1;
        for (int i = 0; i < size; i++) {
            if (services[i].getPrice() > priceLimit) {
                indexToDelete = i;
                break;
            }
        }

        if (indexToDelete == -1) {
            // �� �������� ������ �� ����������� ���������
            throw runtime_error("No service found with the specified criteria.");
        }

        Service* newServices = new Service[size - 1];
        for (int i = 0, j = 0; i < size; i++) {
            if (i != indexToDelete) {
                newServices[j++] = services[i];
            }
        }

        delete[] services;
        services = newServices;
        size--;
    }
    catch (const bad_alloc& exception) {
        cerr << "Memory allocation error: " << exception.what() << endl;
    }
    catch (const runtime_error& exception) {
        cerr << "Runtime error: " << exception.what() << endl;
    }
}

// ��������� � ���������� ������ �� ������ �������
void Service_Static(Service services[], int& size, double priceLimit) {
    try {
        if (size == 0) {
            // ��������� ����� ��������. ���� �������� ��� ���������
            throw runtime_error("Static array is empty. No items to delete.");
        }

        int indexToDelete = -1;
        for (int i = 0; i < size; i++) {
            if (services[i].getPrice() > priceLimit) {
                indexToDelete = i;
                break;
            }
        }

        if (indexToDelete == -1) {
            // �� �������� ������ �� ����������� ���������
            throw runtime_error("No service found with the specified criteria.");
        }

        for (int i = indexToDelete; i < size - 1; i++) {
            services[i] = services[i + 1];
        }
        size--;
    }
    catch (const runtime_error& exception) {
        cerr << "Runtime error: " << exception.what() << endl;
    }
}

// 5.8
void displayMenu() {
    cout << "\n--- Hotel Booking System Menu ---\n";
    cout << "1. Create a Booking Request\n";
    cout << "2. Delete a Booking Request\n";
    cout << "3. Create a Service\n";
    cout << "4. Delete a Service\n";
    cout << "5. Exit\n";
    cout << "Select an option: ";
}

// 5.11
// ����� ��� ������������ ������ ���������� �������
void displayServiceInfo(Service* service) {
    // ������ ������ ������, ��� ��� ����� ���� ��'����
    service->displayService();
}

int main(int argument_�ount, char* argument_vector[]) {
    setlocale(LC_ALL, "ukr");

    //5.7
    // cd ... Laboratory_Project_OOP.exe service.txt
    // �������� �������� ��������� ���������� �����
    //if (argument_�ount < 2) {
    //    cerr << "Please provide the filename as a command-line argument." << endl;
    //    return 1;
    //}

    //string filename = argument_vector[1];

    //// ������� ��������� ��'���� Service
    //Service service("Room Cleaning", 25.0);
    //// ������ ������� � ������ � ������
    //ServiceFileOperations(service, filename);
    //cout << "_________________________" << endl;
    //cout << endl;

// 5.8
    cout << "___________________________________" << endl;
    bool isRunning = true;
    int userChoice;

    vector<BookingRequest> bookingRequests;
    vector<Service> services;

    while (isRunning) {
        displayMenu();
        cin >> userChoice;

        try {
            switch (userChoice) {
            case 1: {
                // ��������� ������ BookingRequest
                string name;
                int guests, nights;
                double cost;
                string roomType;

                cout << "Enter guest name: ";
                cin >> name;
                cout << "Enter number of guests: ";
                cin >> guests;
                cout << "Enter room type: ";
                cin >> roomType;
                cout << "Enter number of nights: ";
                cin >> nights;
                cout << "Enter cost per night: ";
                cin >> cost;

                BookingRequest newRequest(name, guests, roomType, nights, cost);
                bookingRequests.push_back(newRequest);

                cout << "Booking Request created successfully." << endl;
                break;
            }
            case 2: {
                // ��������� BookingRequest �� ��������
                if (bookingRequests.empty()) {
                    cout << "No booking requests available to delete.\n";
                    break;
                }

                int index;
                cout << "Enter the index of the Booking Request to delete (0 to " << bookingRequests.size() - 1 << "): ";
                cin >> index;

                // ����������, �� � �������� ������ � ����� ��������
                if (cin.fail() || index < 0 || index >= bookingRequests.size()) {
                    cin.clear(); // ������� ���� cin, ���� ��� �����������
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ��������� ������� �����
                    cout << "Invalid index. Unable to delete Booking Request.\n";
                }
                else {
                    bookingRequests.erase(bookingRequests.begin() + index);
                    cout << "Booking Request deleted successfully.\n";
                }
                break;
            }
            case 3: {
                // ��������� ������ Service
                string serviceName;
                double price;

                cout << "Enter service name: ";
                cin >> serviceName;
                cout << "Enter service price: ";
                cin >> price;

                Service newService(serviceName, price);
                services.push_back(newService);

                cout << "Service created successfully." << endl;
                break;
            }
            case 4: {
                // ��������� Service �� ��������
                if (services.empty()) {
                    cout << "No services available to delete.\n";
                    break;
                }

                int index;
                cout << "Enter the index of the Service to delete (0 to " << services.size() - 1 << "): ";
                cin >> index;

                // ���������� ����������� �������
                if (cin.fail() || index < 0 || index >= services.size()) {
                    cin.clear(); // ������� ���� cin �� ����������� �����
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ��������� ������� �����
                    cout << "Invalid index. Unable to delete Service.\n";
                }
                else {
                    services.erase(services.begin() + index);
                    cout << "Service deleted successfully.\n";
                }
                break;
            }

            case 5: {
                // ����� �� ���� 
                isRunning = false;
                cout << "Exit the menu..." << endl << endl;
                break;
            }
            default: {
                cerr << "Invalid choice. Please try again." << endl;
                break;
            }
            }
        }
        catch (const out_of_range& exception) {
            cerr << "Error: " << exception.what() << "\n";
        }
        catch (const exception& exception) {
            cerr << "An unexpected error occurred: " << exception.what() << "\n";
        }
    }
    cout << "___________________________________" << endl;
    cout << endl;

    // 5.11
    cout << "_____________5.11_______________" << endl;
    Service* room_service = new RoomService("Mini bar", 10.0);
    // ³��������� ����� ��������� �������
    cout << "We display the bypass of the virtual function:" << endl;
    // ������������ ������� ����� ������� ����
    displayServiceInfo(room_service);  // ����������� displayService() ��� RoomService


    // 5.10
    cout << "_____________5.10_______________" << endl;
    RoomService roomService("Breakfast on the terrace", 55.0);
    roomService.displayService();  // ����� ��'�������� (��������)
    cout << "Cost: " << roomService.calculateCost() << endl;

    // 5.9, 5.10
    // ����� 1: ������������ ��������� �� ������� ����
    cout << "_____________5.9_______________" << endl;
    Service* maintenance1 = new RoomService("Breakfast in Room", 20.0);
    Service* maintenance2 = new RoomService("Dinner Service", 45.0);

    maintenance1->displayService(); // ϳ��� ��'�������� (��������)

    // ����� 2: ������������ ������ ��������� �� ������� ����
    Service* maintenance[2];
    maintenance[0] = new RoomService("Dinner Service", 45.0);
    maintenance[1] = new RoomService("Breakfast in Room", 20.0);

    for (int i = 0; i < 2; ++i) {
        maintenance[i]->displayService();  // ������ ���������� ������
    }

    // ����� 3: �������������� ������ �� ������������ ���� ����� ��'����
    Service* dynamicService = new RoomService("Late Checkout", 25.0);
    dynamicService->displayService();

    // ���� ����� ��'���� �� ��������� �����������
    delete dynamicService;

    dynamicService = new RoomService("Cleaning of the room", 55.0);
    dynamicService->displayService();
    cout << "____________________________" << endl;
    cout << endl;

    //5.6
    // ��������� ����� Service
    int dynamicSize = 5;
    Service* dynamicServices = new Service[dynamicSize]{
        Service("Breakfast in room", 15.0),
        Service("Transfer", 30.0),
        Service("Pool Access", 20.0),
        Service("Spa Treatment", 50.0),
        Service("Dinner Package", 40.0)
    };
    cout << endl;
    // ��������� ����� Service
    int staticSize = 5;
    Service staticServices[5] = {
        Service("Breakfast in room", 15.0),
        Service("Transfer", 30.0),
        Service("Pool Access", 20.0),
        Service("Spa Treatment", 50.0),
        Service("Dinner Package", 40.0)
    };

    cout << "_________________________________________" << endl;
    // ������������ ������ ��������� � ������� ����: ���� ���� ������� ����� �� 25, ������� ����������� 
    double priceLimit = 25.0;

    cout << "Removing from dynamic array..." << endl;
    Service_Dynamic(dynamicServices, dynamicSize, priceLimit);

    cout << "Removing from static array..." << endl << endl;
    Service_Static(staticServices, staticSize, priceLimit);
    cout << "_________________________________________" << endl;

    // 5.4
    // ³��������� ����� ��� ��������� ������� ���������� ��� ����������
    BookingRequest getbookingsummary("Andrii Kharakternyk", 2, "Luxury", 5, 1200.0);
    cout << getbookingsummary.getBookingSummary() << endl << endl;

    VIPCustomer personalAssistant;
    // ������ ��� ���������
    cout << "List of Personal Assistants:" << endl;
    for (int i = 0; i < 5; i++) {
        cout << "Personal Assistant " << (i + 1) << ": " << personalAssistant.getPersonalAssistant(i) << endl;
    }
    cout << endl;

    // 5.1,5.2
    double basePrice = 200.0;

    SilverProgram silver;
    GoldProgram gold;
    PlatinumProgram platinum;
    cout << "|__Abstract class hierarchy__|";
    cout << "\nOriginal Price: $" << basePrice << endl;

    // ������������ SilverProgram
    silver.displayProgramInfo();
    cout << "Price after Silver discount: $" << silver.calculateDiscount(basePrice) << endl;

    // ������������ GoldProgram
    gold.displayProgramInfo();
    cout << "Price after Gold discount: $" << gold.calculateDiscount(basePrice) << endl;

    // ������������ PlatinumProgram
    platinum.displayProgramInfo();
    cout << "Price after Platinum discount: $" << platinum.calculateDiscount(basePrice) << "\n" << endl;



    // 5,6
    // ��������� ��'��� VIPCustomer
    VIPCustomer vipCustomer("Alice Johnson", "alice@vip.com", 3, " ");
    cout << "\n----- VIP Customer Info -----" << endl;
    vipCustomer.displayCustomerInfo();

    // ������������ ������������ ������
    double originalPrice = 500.0;
    double discountedPrice = vipCustomer.applyDiscount(originalPrice);
    cout << "Original Price: $" << originalPrice << endl;
    cout << "Discounted Price for VIP Customer: $" << discountedPrice << "\n" << endl;
    // ������������ ������� showBaseClassAccess ��� ������� �� ������ �������� �����
    vipCustomer.showBaseClassAccess();



    // 4
    // �������� ��������� ��������� �����������

        // ��������� ��'���� SpecialCustomer
    cout << "Creating a  SpecialCustomer object...\n";
    SpecialCustomer specialCustomer("John Smith", "john@special.com", 150, true, "VIP Lounge Access", 25.0, true);
    cout << "Exiting main...\n";
    cout << "\n----- Special Customer Info -----" << endl;
    specialCustomer.displaySpecialCustomerInfo();

    double discountedPrice_ = specialCustomer.calculateDiscountedServicePrice(0.2); // 20% ������
    cout << "Discounted Service Price: $" << discountedPrice_ << endl << endl;


    cout << "*********New_updates********* " << endl;
    // ��������� ��'���� VIPCustomer
    cout << "Creating a  VIPCustomer object...\n";
    VIPCustomer vipClient("Alice Johnson", "alice@vip.com", 5, "Emily Smith", true);
    cout << "Exiting main...\n";
    vipClient.displayCustomerInfo();
    cout << endl;

    // ��������� ��'���� PremiumService
    PremiumService vipService("Personal Spa Session", 100.0, "Sarah Connor", 50);
    cout << vipService.getServiceInfo() << ", Price: $" << vipService.calculateDiscountedPrice(0.1) << endl << endl;

    // ��������� ��'���� SuiteRoom
    SuiteRoom suiteRoom(4, 200.0, true, true, "Executive Suite");
    suiteRoom.displayRoomInfo();

    // ��������� �� ��������� ���������� ��� ���������� VIP-�볺���
    BookingRequest bookingRequest("Alice Johnson", 2, "Suite", 3, suiteRoom.calculateCost(3));
    cout << "Total Bookings: " << BookingRequest::getTotalBookings() << endl << endl;

    cout << "***************************** " << endl;

    // ��������� ��'���� �������
    Service breakfast("Breakfast in room", 15.0);
    // ��������� ���������� ��� �������
    cout << "Service Name: " << breakfast.getServiceName() << endl;
    cout << "Service Price: $" << breakfast.getPrice() << endl;
    cout << "Service Type: " << breakfast.getServiceType() << "\n" << endl;
    cout << "\n********************************" << endl;
    cout << "************11,12,13************" << endl;
    // ��������� ��'���� Customer
    Customer customer("John Doe", "john@example.com");
    customer.displayCustomerInfo();

    // ��������� ��'���� Room
    Room rooms1(2, "Standard", 50.0);
    Room rooms2(3, "Deluxe", 80.0);

    // ��������� ��'���� Hotel �� ��������� �����
    Hotel hotel("Grand Hotel");
    hotel.addRoom(rooms1);
    hotel.addRoom(rooms2);
    hotel.displayRooms();

    // ��������� ������ �� ����������
    BookingRequest bookingRequest_("John Doe", 2, "Standard", 3, rooms1.calculateCost(3));
    Booking booking(bookingRequest, rooms1);

    // ��������� ������ �� ����������
    Service breakfast_t("Breakfast", 15.0);
    Service transfer_r("Airport Transfer", 30.0);

    booking.addService(breakfast_t);
    booking.addService(transfer_r);

    // ��������� ������� ����������
    booking.displayBookingDetails();
    cout << "********************************\n" << endl;



    // ��������� � ��������� ��'��� Room �� ��������� ������� createDeluxeRoom
    Room myRoom = createDeluxeRoom();
    // ��������� ��������� ����� ��� ��������
    if (!myRoom.checkAvailability()) {
        cout << "The room has been booked successfully" << endl;
    }
    cout << "\n---------Simple method---------" << endl;
    // 1. ������� �����
    Room simpleRoom(2, "Standard", 50.0);  // ������ ��������� ��'����
    simpleRoom.displayRoomInfo();
    cout << "---------Explicit method---------" << endl;
    // 2. ����� �����
    Room explicitRoom = Room(3, "Deluxe", 80.0);  // ����� ������ ������������
    explicitRoom.displayRoomInfo();
    cout << "---------Shorted method---------" << endl;
    // 3. ���������� �����
    Room shortRoom{ 4, "Suite", 120.0 };  // ������ ������������
    shortRoom.displayRoomInfo();

    cout << "\n\n *|___System Hotel___|*" << endl << endl;
    // ������ ���������� ��� ������
    Room room1(2, "Standard", 50.0);
    Room room2(3, "Deluxe", 80.0);
    Room room3(4, "Suite", 120.0);


    // 1. ��������� ���������� ������ ������
    try {
        Service* services = nullptr;

        // ������������ try-catch ��� ������� ������� � ������� ������� �������� ���'��
        try {
            services = new Service[5];
        }
        catch (bad_alloc& exception) {
            //������� �������� ���'�� ��� ������ ������
            cerr << "Error allocating memory for services array: " << exception.what() << endl;
            return 1;
        }
        services[0] = Service("Breakfast in room", 15.0);
        services[1] = Service("Transfer", 30.0);
        services[2] = Service("Pool Access", 20.0);
        services[3] = Service("Spa Treatment", 50.0);
        services[4] = Service("Dinner Package", 40.0);

        // 2. �������� �� ������������ ������� ������
        try {
            int index = 5; // ������������� ������
            if (index < 0 || index >= 5) {
                // I����� ������ �������� �� ���i ����������� ��������
                throw out_of_range("The index of the array is out of range\n");
            }
            cout << "Service: " << services[index].getServiceName() << endl;
        }
        catch (const out_of_range& exception) {
            cerr << "\n\nException: " << exception.what() << endl;
        }



        // ��������� ����� �� 5 �볺���
        BookingRequest static  clients[5] = {
            BookingRequest("Alice Smith", 2, "Standard", 3, room1.calculateCost(3)),
            BookingRequest("Bob Johnson", 3, "Deluxe", 4, room2.calculateCost(4)),
            BookingRequest("Charlie Brown", 2, "Standard", 2, room1.calculateCost(2)),
            BookingRequest("David Williams", 4, "Suite", 1, room3.calculateCost(1)),
            BookingRequest("Eve Miller", 3, "Deluxe", 5, room2.calculateCost(5))
        };


        // 4 �����
        //Room room1(2, "Standard", 50.0);
        //BookingRequest request("Alice Smith", 2, "Standard", 3, room1.calculateCost(3), 101, room1);
        //
        //
        //// ��������� ���������� ��� ����������
        //request.displayBookingInfo();
        //cout << "Booking ID: " << request.getBookingID() << endl;
        //cout << "Booked Room Info:" << endl;
        //request.getBookedRoom().displayRoomInfo();

        //  ���� �������� ������� ���������
        cout << "------------------" << endl;
        cout << "Total Bookings: " << BookingRequest::getTotalBookings() << endl;
        cout << "------------------" << endl;
        // ������� ������� ���������
        for (int i = 0; i < 5; i++) {
            displayBookingSummary(clients[i]);
        }
        // ������ ������������� ������� � ������������ ������
        for (int i = 0; i < 5; i++) {
            displayBookingSummary(clients[i], 0.15); // ���������, 15% ������
        }
        // ������� ���� � �������� ������ 
        BookingRequest copiedClient = clients[0];
        // �������� ���������� ��� ����������� �� ����������� �������� �� ����������
        cout << "__________________________________" << endl;
        cout << "Original Booking:" << endl;
        clients[0].displayBookingInfo();
        cout << "\nCopied Booking:" << endl;
        copiedClient.displayBookingInfo();
        cout << "__________________________________\n\n" << endl;



        // ��������� ���������� ��� ������� �볺��� �� ������������ ����������
        cout << "Booking Requests Before Confirmation:\n" << endl;
        for (int i = 0; i < 5; i++) {
            clients[i].displayBookingInfo();
        }
        cout << endl;

        // ��������� ��������� ������ ��� ������� �볺���
        for (int i = 0; i < 5; i++) {
            int randomServiceCount = rand() % 5; // ��������� ����� �� 0 �� 2 (������� ������ ��� �볺���)

            if (randomServiceCount > 0) clients[i].addService(services[0]); // ������ breakfast, ���� > 0
            if (randomServiceCount > 1) clients[i].addService(services[1]); // ������ transfer, ���� > 1
            if (randomServiceCount > 3) clients[i].addService(services[3]); // ������ Spa Treatment ���� > 3
            if (randomServiceCount > 1) clients[i].addService(services[4]); // ������ Dinner Package ���� > 1
            if (rand() % 2 == 0) clients[i].addService(services[2]);        // ������ poolAccess ��������� (50% ����)
        }


        // ϳ����������� ���������� ��� ��� �볺��� �� ��������� ���������� ���� ������������
        cout << "\nBooking Requests After Confirmation:" << endl;
        for (int i = 0; i < 5; i++) {
            clients[i].confirmBooking();
        }

        // ���������� ���������� � ����
        saveClientsToFile(clients, 5, "clients.txt");

        // ������������ �볺��� � ����� �� ��������� ����������
        cout << "Loaded Booking Requests:" << endl;
        BookingRequest::loadClientsFromFile(clients, 5, "clients.txt");
        for (int i = 0; i < 5; i++) {
            clients[i].displayBookingInfo();
        }



        // 3. ��������� ������ � �������� ���������� ��� ���������� ��������� ��������� ���������
        try {
            delete[] services;
            services = nullptr; // ��������� ���������

            // �������� ��������� ��������� ���������
            if (!services) {
                // ������ ��������� ��������� ���������
                throw runtime_error("Attempting to delete a null pointer");
            }
            delete[] services;
        }
        catch (const runtime_error& exception) {
            cerr << "\nException: " << exception.what() << endl << endl;
        }
    }
    catch (const exception& exception) {
        // �������� �������
        cerr << "Common error: " << exception.what() << endl;
    }

    // ��������� �������� ���'��
    delete[] dynamicServices;
    delete room_service;
    // 5.9
    // ��������� ���'��
    delete maintenance1;
    delete maintenance2;
    delete maintenance[0];
    delete maintenance[1];
    delete dynamicService;
    return 0;
}