#!/bin/bash

echo "Content-type: text/html"
echo ""

# Get query string from environment variable
QUERY_STRING=${QUERY_STRING:-"voice="}
VOICE=${QUERY_STRING#voice=}

# URL decode the numbers
VOICE=$(echo "$VOICE" | sed 's/+/ /g' | sed 's/%20/ /g')

echo "<html>"
echo "<head><title>Push Swap Result</title></head>"
echo "<body>"
echo "<div class='output-container'>"

echo "<h3>Input:</h3>"
echo "<pre>$VOICE</pre>"
echo "<h3>Output:</h3>"
echo "<pre>"
# Execute push_swap with the provided numbers
./www/cgi-bin/megaphone "$VOICE" 2>&1
echo "</pre>"

echo "</div>"
echo "</body></html>"