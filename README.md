# *Client-Server Shopping Application*

## *Overview*

This project is a client-server application that manages user authentication and product data storage. It is designed using various software design patterns, ensuring a scalable, maintainable, and robust architecture. The system allows clients to register, log in, and manage their product inventory, with data stored in a structured format. The communication between the client and server is handled using TCP for reliable messaging, and UDP is used for broadcasting messages to clients.

## *Features*

- *User Authentication*: Secure client registration and login using a username and password.
- *Product Management*: Clients can add products to their inventory, and this data is persistently stored in CSV files.
- *Design Patterns*: 
  - *Factory Pattern*: Used for creating objects without specifying the exact class.
  - *Singleton Pattern*: Ensures that only one instance of shopping server exists throughout the application.
  - *Command Pattern*: Encapsulates requests as objects, allowing for parameterization and queuing of requests from client.
  - *Bridge Pattern*: Decouples an abstraction from its implementation, enabling them to vary independently. Used in payment where card & upi implementation is hidden.
  - *Prototype Pattern*: Creates new objects by copying existing ones, improving efficiency in object creation. Used when clonning the product to client's cart. 
  - *Decorator Pattern*: Allows behavior to be added to individual objects, dynamically extending functionality without modifying the original code. Used in discounting a product before purchase.
- *Logging*: Logs all significant events, such as client connections, registrations, and errors, to a log file for easy debugging and monitoring.
- *Communication Protocols*: 
  - *TCP*: Used for reliable client-server communication.
  - *UDP*: Employed for broadcasting messages to all clients about the discount for products more than 1000 price.
- *Client Data Isolation*: Each client has a separate CSV file to store their product data, ensuring data isolation and ease of access.

## *How to Build and Run*

1. Download shopping server and client as separate projects.
2. Run shopping server first it ensure the server runs properly with tcp and udp in separate ports.
3. Then run the client project which has client file.
4. I have added one client, like this multiple clients can connect at a time since it supports multithreading.

### *Usage*

1. *Register a New Client*:
   - A new client can be registered by sending a register command followed by a username and password.
   - Example: register:username,password

2. *Login a Client*:
   - A client can log in using the login command followed by their username and password.
   - Example: login:username,password

3. *Manage Products*:
   - Once logged in, a client can view product, add products to cart, delete products from the cart, view the cart, apply discount, checkout the cart and perform the payment(card/upi).
   - Products are stored in their respective CSV file, which is located in the client_data/ directory.
  
4.  *Broadcasting*:
   - Server broadcasts messages every 10 seconds to all connected clients using UDP about the discount for products more than 1000 price.

## *Thread Safety*

- The project ensures thread safety by using std::mutex to lock critical sections, such as file access and client data management. This prevents race conditions when multiple clients interact with the server simultaneously.
