#!/bin/bash

echo "Content-type: text/html"
echo ""

# Read the POST data from stdin
read -n "$CONTENT_LENGTH" POST_DATA

# Extract the "numbers" value from POST data
NUMBERS=$(echo "$POST_DATA" | sed -n 's/^numbers=//p')

# URL decode (replace + and %20 with spaces)
NUMBERS=$(echo "$NUMBERS" | sed 's/+/ /g' | sed 's/%20/ /g')

echo "<html>"
echo "<head><title>Push Swap Result</title></head>"
echo "<body>"
echo "<div class='output-container'>"

if [ -z "$NUMBERS" ]; then
    echo "<p class='error'>No numbers provided</p>"
else
    echo "<h3>Input:</h3>"
    echo "<pre>$NUMBERS</pre>"
    echo "<h3>Output:</h3>"
    echo "<pre>"
    ./www/cgi-bin/push_swap $NUMBERS 2>&1
    echo "</pre>"
fi

echo "</div>"
echo "</body></html>"
