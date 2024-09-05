import socket

# Define server address and port
server_address = ('localhost', 12000)

# Create a socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


def authenticate_user(username, password):
    # Construct authentication message
    message = f"AUTH|{username}|{password}"
    # Send authentication request to server
    client_socket.sendto(message.encode(), server_address)

    # Receive response from server
    response, _ = client_socket.recvfrom(2048)
    print("Server Response:", response.decode())  # Debugging: Print server response
    return response.decode().startswith("Authenticated")


def make_transaction(username):
    while True:
        # Fetch user's transactions from server
        transaction_info = fetch_transactions(username)
        balance, transactions = transaction_info.split("|")
        transactions = transactions.split(",")

        # Find highest transaction ID where the user is a payer
        highest_tx_id = 0
        for tx in transactions:
            if tx.startswith(f"{username}"):
                tx_id = int(tx.split(":")[0])
                highest_tx_id = max(highest_tx_id, tx_id)

        # Determine new transaction ID
        if highest_tx_id == 0:
            if username == "A":
                new_tx_id = 100
            elif username == "B":
                new_tx_id = 200
            elif username == "C":
                new_tx_id = 300
            elif username == "D":
                new_tx_id = 400
        else:
            new_tx_id = highest_tx_id + 1

        print(f"New Transaction ID: {new_tx_id}")

        # Ask for transaction details
        amount = int(input("Enter amount to transfer: "))
        if username == "A":
            payee1 = input("Enter Payee1 (B, C, or D): ").upper()
        elif username == "B":
            payee1 = input("Enter Payee1 (A, C, or D): ").upper()
        elif username == "C":
            payee1 = input("Enter Payee1 (A, B, or D): ").upper()
        elif username == "D":
            payee1 = input("Enter Payee1 (A, B, or C): ").upper()

        amount_payee1 = int(input(f"Enter amount {payee1} will receive: "))

        # Check if there's a second payee
        if amount - amount_payee1 > 0:
            if username == "A":
                if payee1 == "B":
                    payee2 = input("Enter Payee2 (C, or D): ").upper()
                elif payee1 == "C":
                    payee2 = input("Enter Payee2 (B, or D): ").upper()
                elif payee1 == "D":
                    payee2 = input("Enter Payee2 (B, or C): ").upper()
            elif username == "B":
                if payee1 == "A":
                    payee2 = input("Enter Payee2 (C, or D): ").upper()
                elif payee1 == "C":
                    payee2 = input("Enter Payee2 (A, or D): ").upper()
                elif payee1 == "D":
                    payee2 = input("Enter Payee2 (A, or C): ").upper()
            elif username == "C":
                if payee1 == "A":
                    payee2 = input("Enter Payee2 (B, or D): ").upper()
                elif payee1 == "B":
                    payee2 = input("Enter Payee2 (A, or D): ").upper()
                elif payee1 == "D":
                    payee2 = input("Enter Payee2 (A, or B): ").upper()
            elif username == "D":
                if payee1 == "A":
                    payee2 = input("Enter Payee2 (B, or C): ").upper()
                elif payee1 == "B":
                    payee2 = input("Enter Payee2 (A, or C): ").upper()
                elif payee1 == "C":
                    payee2 = input("Enter Payee2 (A, or B): ").upper()
            else:
                payee2 = None
                payee2_amount = None

            amount_payee2 = amount - amount_payee1
            print(f"{payee2} will receive: {amount_payee2}")

        else:
            payee2 = None
            amount_payee2 = None

        # Construct transaction message
        if payee2:
            message = f"TX|{new_tx_id}|{username}|{amount}|{payee1}|{amount_payee1}|{payee2}|{amount_payee2}"
        else:
            message = f"TX|{new_tx_id}|{username}|{amount}|{payee1}|{amount_payee1}"

        # Send transaction request to server
        client_socket.sendto(message.encode(), server_address)
        print("Transaction request sent to server.")  # Debugging: Print transaction request sent message

        # Receive response from server
        response, _ = client_socket.recvfrom(2048)
        print("Server Response:", response.decode())  # Debugging: Print server response

        # Fetch and display the updated list of transactions
        fetch_and_display_transactions(username)

        # Prompt user if they want to make another transaction
        choice = input("Do you want to make another transaction? (yes/no): ").lower()
        if choice != "yes":
            break

def fetch_transactions(username):
    # Construct fetch message
    message = f"FETCH|{username}"
    # Send request to fetch transactions to server
    client_socket.sendto(message.encode(), server_address)

    # Receive response from server
    response, _ = client_socket.recvfrom(2048)
    return response.decode()

def fetch_and_display_transactions(username):
    # Fetch and display the list of transactions
    transaction_info = fetch_transactions(username).strip()  # Strip whitespace
    print("Transaction info received:", transaction_info)
    parts = transaction_info.split("|")
    print("Length of parts:", len(parts))  # Add this line
    if len(parts) == 2:
        balance = parts[0]
        transactions = parts[1]
        print("Transactions:", transactions)  # Add this line
        if transactions:  # Check if transactions are not empty
            transactions = transactions.split(",")
            print("Individual transactions:", transactions)  # Add this line
            print(f"Your current balance is {balance} BTC.")
            print("List of transactions:")
            for transaction in transactions:
                print(transaction)

        else:
            print("Unexpected format of transaction info:", transaction_info)
        # Handle the unexpected format gracefully


# Example usage
if __name__ == "__main__":
    authenticated = False
    while not authenticated:
        username = input("Enter username: ")
        password = input("Enter password: ")

        # Authenticate user
        authenticated = authenticate_user(username, password)

        if not authenticated:
            print("Authentication failed.")
            choice = input("Do you want to quit the program? (yes/no): ")
            if choice.lower() == "yes":
                break

    if authenticated:
        while True:
            # Display menu options
            print("\nMenu:")
            print("1: Make a transaction")
            print("2: Fetch and display the list of transactions")
            print("3: Quit the program")

            # Get user's choice
            choice = input("Enter your choice (1/2/3): ")

            if choice == "1":
                make_transaction(username)
            elif choice == "2":
                fetch_and_display_transactions(username)
            elif choice == "3":
                print("Quitting the program.")
                break
            else:
                print("Invalid choice.")

# Close the socket
client_socket.close()
