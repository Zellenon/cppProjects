/*
 * Basic URL Parser
 * Graham Scott
 * 02.05.2020
 * Version 1.0.5
 * Keeps prompting the user for a URL and returns the scheme, host, and path of the URL provided. Exits when 'quit' is entered
 * Accepts no arguments and simply returns 0.
 */

#include <iostream>

using namespace std;

int main() {
    enum state{SCHEME_START_STATE, SCHEME_STATE, PATH_HOST_STATE, HOST_STATE, PATH_START_STATE, PATH_STATE};
    /*
     * All possible states of the URL State Machine.
     * Scheme Start State is the very beginning of the state machine
     * Scheme State continues while it reads alpha characters until it finds a colon, then moves on to Path/Host
     * Path/Host State is a transitory state used to determine whether the machine is reading a host or a path
     * Host state continues while it reads alpha characters until it finds a colon, then moves on to Path/Host
     * Path Start State is used to rule out characters the parser can't handle, and adjust the index of the reader if necessary.
     * Path State is the final state, and adds any characters except '?' and '#' to the path.
     */
    string input = "";

    cout << "Basic URL Parser. Breaks entered URLS into scheme, host, and path." << endl;
    cout << "Enter URLs in the following format: scheme://host/path" << endl;
    cout << "------------------------------------------------------------------" << endl;
    while (input != "quit") {
        int index = 0;
        cout << "Enter a URL: (enter 'quit' to end program)" << endl;
        cin >> input;
        string buffer = "", scheme = "", host = "", path = "";
        bool error = false; //used as an emergency exit to the state machine when it finds something it can't handle
        state curState = SCHEME_START_STATE;
        while (index < input.length()+1 && index > -1 && !error) {
            char c = input[index];

            if (curState == SCHEME_START_STATE) { //allowed characters: A-Z, a-z
                if (c >= 'A' && c <= 'Z') //insures that if c is an alphachar, it is lowercase
                    c = c + 'a'-'A';
                if (c >= 'a' && c <= 'z') {
                    buffer += c; //adds all allowed characters to the buffer
                    curState = SCHEME_STATE;
                } else {
                    error = true;
                }
            } else if (curState == SCHEME_STATE) { //allowed characters: A-Z, a-z, :, -, +, .
                if (c >= 'A' && c <= 'Z') //insures that if c is an alphachar, it is lowercase
                    c = c + 'a'-'A';
                if ((c >= 'a' && c <= 'z') || c=='+' || c=='-' || c=='.')
                    buffer += c;
                else if (c == ':') { //a colon sends the state machine to the next phase, and dumps buffer into scheme as long as it's followed by a /
                    scheme = buffer;
                    buffer = "";
                    if (index < input.length()){
                        index++;
                        if (input[index] == '/')
                            curState = PATH_HOST_STATE;
                        else
                            error = true;
                    }
                } else
                    error = true;
            } else if (curState == PATH_HOST_STATE) { // see state description
                if (c == '/')
                    curState = HOST_STATE;
                else {
                    curState = PATH_STATE;
                    index--;
                }
            } else if (curState == HOST_STATE) { //allowed characters: any. / or \0 sends state machine to next phase.
                if (c == '\0' || c == '/') {
                    index--;
                    if (buffer.length() == 0) //having triple slashes after the colon (:///) throws an error
                        error = true;
                    else {
                        host = buffer;
                        buffer = "";
                        curState = PATH_START_STATE;
                    }
                } else {
                    buffer += c;
                }
            } else if (curState == PATH_START_STATE) {
                if (scheme == "ftp" || scheme == "http" || scheme == "https") { // these three schemes are considered 'special'
                    curState = PATH_STATE;
                    if (c != '/')
                        index--;
                } else if (c == '?' || c == '#')
                    error = true;
                else if (c != '\0') {
                    curState = PATH_STATE;
                    if (c != '/')
                        index--;
                }
            } else if (curState == PATH_STATE) { //accepts all characters except ? and #. \0 will exit the machine
                if (c == '\0' || c == '?' || c == '#') { //end the Path phase, and thus the state machine
                    path = buffer;
                    buffer = "";
                    if (c != '\0')
                        error = true;
                } else {
                    buffer += c;
                }
            }

            index++;
        } // end of state machine loop
        if (error) {
            cout << "Validation Error: invalid string entered" << endl;
        } else {
            cout << "URL Data:" << endl;
            cout << "Scheme: " << scheme << endl;
            cout << "Host: " << host << endl;
            cout << "Path: " << path << endl;
        }
    }
    return 0;
}
