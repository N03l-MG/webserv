#!/bin/bash

start_time=$(date +%s)

while true; do

    echo "Content-type: text/html"
    echo ""

    echo "<html>"
    echo "<head><title>Limited Loop</title></head>"
    echo "<body>"
    echo "<p>This script will run for 10 seconds and then exit.</p>"
    echo "</body></html>"

    # Sleep for a short duration to avoid excessive CPU usage
    sleep 1
done