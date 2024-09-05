import socket

# Define server address and port
server_address = ('localhost', 12000)

# Create a socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(server_address)

# Dummy database to store user balances
user_balances = {
    "A": {"balance": 10, "password": "A"},
    "B": {"balance": 10, "password": "B"},
    "C": {"balance": 10, "password": "C"},
    "D": {"balance": 10, "password": "D"}
}

# Dummy database to store confirmed transactions
confirmed_transactions = []

# Notify that the server is ready to receive
print('The server is ready to receive')


# Authenticate user
def authenticate_user(username, password):
    if username in user_balances and user_balances[username]["password"] == password:
        response = f"Authenticated: Your current balance is {user_balances[username]['balance']} BTC."
        return True, response
    else:
        return False, "Authentication failed."


# Function to handle transaction request
def handle_transaction(message):
    # Split the transaction message by '|'
    parts = message.split('|')

    # Extract relevant information from parts
    action = parts[0]
    username = parts[2]

    # Ensure the username is a single character
    if len(username) != 1:
        return "TX Rejected: Invalid username."

    # Assign transaction ID based on username
    tx_id = 100 + ord(username.upper()) - ord('A')  # Assign transaction ID based on username

    # Extract other transaction details
    payer = username  # Use the authenticated username here
    amount = int(parts[3])
    payee1 = parts[4]
    amount_payee1 = int(parts[5])
    payee2 = parts[6] if len(parts) > 6 else None
    amount_payee2 = int(parts[7]) if len(parts) > 7 else None

    if user_balances[payer]["balance"] < amount:
        # Insufficient balance
        return f"TX Rejected: Insufficient balance. Your current balance is {user_balances[payer]['balance']} BTC."
    else:
        # Sufficient balance
        # Update balances
        user_balances[payer]["balance"] -= amount
        user_balances[payee1]["balance"] += amount_payee1
        if payee2:
            user_balances[payee2]["balance"] += amount_payee2

        # Store transaction
        transaction_entry = f"{tx_id}|{payer}|{amount}|{payee1}|{amount_payee1}|{payee2}|{amount_payee2 if payee2 else ''}"
        print("Storing transaction:", transaction_entry)
        confirmed_transactions.append(transaction_entry)
        return f"TX Confirmed: Your current balance is {user_balances[payer]['balance']} BTC."



while True:
    # Receive request from client
    message, client_address = server_socket.recvfrom(2048)
    message = message.decode()

    print(f"Received message from client: {message}")

    # Parse request
    request_parts = message.split("|")
    action = request_parts[0]
    username = request_parts[1]

    if action == "AUTH":
        print("Attempting authentication...")
        password = request_parts[2]
        username = request_parts[1].strip()
        authenticated, response = authenticate_user(username, password)
        if authenticated:
            print("Authentication successful.")
            server_socket.sendto(response.encode(), client_address)
        else:
            print("Authentication failed.")
            server_socket.sendto(response.encode(), client_address)

    elif action == "TX":
        print("Transaction requested...")
        username = request_parts[2].strip()
        tx_id = 100 + ord(username.upper()) - ord('A')  # Assign transaction ID based on username
        payer = username  # Use the authenticated username here
        amount = int(request_parts[3])
        payee1 = request_parts[4]
        amount_payee1 = int(request_parts[5])
        payee2 = request_parts[6] if len(request_parts) > 6 else None
        amount_payee2 = int(request_parts[7]) if len(request_parts) > 7 else None

        response = handle_transaction(message)
        server_socket.sendto(response.encode(), client_address)
        # Send acknowledgment back to client indicating transaction was processed

    elif action == "FETCH":

        print("Fetch requested...")

# Send user's balance and confirmed transactions to client
        balance = user_balances[username]["balance"]
        user_transactions = [tx for tx in confirmed_transactions if tx.startswith(f"{username}|")]
        transactions_str = ",".join(user_transactions)
        print("Transactions sent to client:", transactions_str)  # Add this line
        response = f"{balance}|{transactions_str}"
        server_socket.sendto(response.encode(), client_address)

# Close the socket
server_socket.close()
