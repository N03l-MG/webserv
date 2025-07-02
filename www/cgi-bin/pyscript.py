#!/usr/bin/env python3

import cgi
import cgitb

cgitb.enable()  # Show errors in browser

print("Content-Type: text/html\n")

form = cgi.FieldStorage()

# Extract parameters
try:
    op1 = float(form.getfirst("operand1", ""))
    op2 = float(form.getfirst("operand2", ""))
    operation = form.getfirst("operation", "")
except ValueError:
    print("<h2>Error: Please enter valid numbers.</h2>")
    exit()

# Perform the calculation
result = None
error = None

if operation == "add":
    result = op1 + op2
elif operation == "subtract":
    result = op1 - op2
elif operation == "multiply":
    result = op1 * op2
elif operation == "divide":
    if op2 == 0:
        error = "Error: Division by zero."
    else:
        result = op1 / op2
else:
    error = "Error: Unknown operation."

# Output result
print("<html><body>")
print("<h1>Simple CGI Calculator</h1>")
print("<p>Operand 1: {}</p>".format(op1))
print("<p>Operand 2: {}</p>".format(op2))
print("<p>Operation: {}</p>".format(operation))

if error:
    print(f"<p style='color:red'>{error}</p>")
else:
    print(f"<p>Result: <strong>{result}</strong></p>")

print("</body></html>")
