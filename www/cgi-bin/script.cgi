#!/bin/bash

echo "Content-type: text/html"
echo ""

# Get query string from environment variable
QUERY_STRING=${QUERY_STRING:-"numbers="}
NUMBERS=${QUERY_STRING#numbers=}

# URL decode the numbers
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
    # Execute push_swap with the provided numbers
    ./www/cgi-bin/push_swap $NUMBERS 2>&1
    echo "</pre>"
fi

echo "</div>"
echo "</body></html>"