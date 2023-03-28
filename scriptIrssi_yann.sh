# Set the host and port
HOST="localhost"
PORT=6061

# Connect to the remote host whith a heredoc in entry
nc $HOST $PORT <<EOF
CAP LS 
PASS 1 
NICK user3 
USER user3 0 *:user3
JOIN #a
PRIVMSG #a :salut

EOF
