# Set the host and port
HOST="localhost"
PORT=6061

# Connect to the remote host whith a heredoc in entry
nc $HOST $PORT <<EOF
CAP LS 
PASS 1 
NICK yann 
USER yaya 0 *:yann
JOIN #chan1
JOIN #chan2
JOIN chan2
EOF
