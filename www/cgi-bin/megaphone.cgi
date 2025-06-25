#!/bin/bash

echo "Content-type: text/html"
echo ""

# Read POST data
read POST_DATA

# Extract the 'input' parameter from POST data
MEGA_INPUT=$(echo "$POST_DATA" | sed 's/^input=//')

echo "<html>"
echo "<head><title>Megaphone Result</title></head>"
echo "<body>"
echo "<div class='output-container'>"

echo "<h3>Input:</h3>"
echo "<pre>$MEGA_INPUT</pre>"
echo "<h3>Output:</h3>"
echo "<pre>"
# Execute megaphone with the provided input
./www/cgi-bin/megaphone "$MEGA_INPUT" 2>&1
echo "</pre>"

echo "</div>"
echo "</body></html>"