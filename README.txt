Communication with REST API.


For parsing JSON files I used "Parson", as suggested in the theme requirement, calling methods: json_parse_string (string) (creates a JSON type value from a string), json_object_get_string (json_object (obj), "numecamp") (extracts the value of the "numecamp" field from the JSON object), json_object_dotget_object (json_object (obj), "numecamp") (extracts a field / object from a JSON value).

The client (client.c) makes GET and POST requests based on the statements received at each task, in the same program running:

- get_ip method - returns the ip address of the received domain as a parameter;
- hand:
- Task 1: Make a Get request at the address / port mentioned in the request;
- Task 2: Extracts the parameters from the response received from the server (cookies, data, ..) and parses the JSON file according to the required information (username, password, ..). It will make a request of type GET / POST (as it is mentioned in the "method" field), with the necessary headers and the value of the cookies received previously to which adds the credentials extracted from the JSON file, the "data" field;
- Task 3: Will perform the steps from task 2, making sure that it adds in addition to the request, the header of "Authorization", in which it completes the value of the token received previously, extracted from the field "data" and updates the "path" to which the request is made with the answers of the guessers;
- Task 4: Will perform the steps from the previous task, making a request with the parameters mentioned in the JSON file;
- Task 5: Extracts the parameters from the response received from the server (cookies, data) and parses the JSON file according to the necessary information. It will extract the domain name whose IP address is to be found, and then do so by the "get_ip" method. A request will be made to this address, respecting the parameters in the response received from the server and adding to the "path" of the request, the parameters "q" and "APPID", with their values, previously received from the server. Resend the response from the domain to the server;

In requests.c you can find the functions that compose the HTTP message, according to the type of request: GET / POST and the required headers.