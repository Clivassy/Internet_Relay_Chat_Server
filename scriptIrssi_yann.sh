# Set the host and port
HOST="localhost"
PORT=6062

# Connect to the remote host whith a heredoc in entry
nc $HOST $PORT <<EOF
CAP LS 
PASS 1 
NICK user1
USER user1 0 *:user1


EOF
