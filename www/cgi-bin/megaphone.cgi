#!/bin/bash

echo "Content-type: text/html"
echo ""

# Get query string from environment variable
QUERY_STRING=${QUERY_STRING:-"input="}
MEGA_INPUT=${QUERY_STRING#input=}

# URL decode the numbers
MEGA_INPUT=$(echo "$MEGA_INPUT" | sed 's/+/ /g' | sed 's/%20/ /g')

echo "<html>"
echo "<head><title>Megaphone Result</title></head>"
echo "<body>"
echo "<div class='output-container'>"

echo "<h3>Input:</h3>"
echo "<pre>$MEGA_INPUT</pre>"
echo "<h3>Output:</h3>"
echo "<pre>"
# Execute push_swap with the provided numbers
./www/cgi-bin/megaphone $MEGA_INPUT 2>&1
echo "</pre>"

echo "</div>"
echo "</body></html>"