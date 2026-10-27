#include <bits/stdc++.h>
using namespace std;

/* Converter Java - PSeInt(Spanish Edition)
Idk, just made this cuz im lazy and my professor wants me to write every program in PSeInt and Java
So this thing does half the work for me :)
It's a really messy code and very hard-coded, I did it in the worst way possible (probably)
Maybe in the future I'll remake it but in a better way, in a not hard-coded way
xd hope it helps you in some way

Pd. The entry has to be just the code inside the "public static void main(String[] args)", (including it)
*/

// Print tabulations
void printTabs(int tabs){
    // Print tabulations equal to the actual identation number
    for(int i=0; i<tabs; i++){
        cout << '\t';
    }
}


void checkCloseStatement(string &line, int &tabs, stack<string> &openStatements, int &currentChar, bool forceClosing){
    // Flag: close an open statement
    bool closeStatement = forceClosing;

    if(!forceClosing){
        // Check every character
        for(char c : line){
            // If we found an open bracket, dont close the statement
            if(c == '{') break;
            // If we found a close bracket before an open one, close the statement
            if(c == '}'){
                closeStatement = true;
                break;
            }
        }
    }

    // Close an open statement
    if(closeStatement){
        // Quit a tabulation
        tabs--;

        // Print tabulations for the new line (except if is a do/while)
        if(openStatements.top() != "dowhile") printTabs(tabs);

        // Check which type of statement we're closing
        if(openStatements.top() == "if") cout << "FinSi" << endl;
        else if(openStatements.top() == "for") cout << "FinPara" << endl;
        else if(openStatements.top() == "while") cout << "FinMientras" << endl;
        else if(openStatements.top() == "dowhile") cout << "";

        // Quit the statement
        openStatements.pop();

        // Move to the current character in the line
        if(!forceClosing) currentChar = line.find("}") != string::npos ? line.find("}") + 1 : 0;
    }
}


// Get variable names
tuple<string, int, string> getVariableName(string line, int pos){
    // To storage: var name, the pos of the next var (-1 if this is the last), var initial value
    tuple<string, int, string> varStorager("", -1, "");

    // Flag: Reading the variable name
    bool readingVarName = true;

    // Read all the variable name
    for(int i=pos; i<line.size(); i++){
        // Read another character to the initial value (if we aren't reading the var name; exclude semicolons and commas)
        if(line[i] != ';' && line[i] != ',' && !readingVarName) get<2>(varStorager) += line[i];

        // If it's text
        if(line[i] != ';' && line[i] != ',' && line[i] != ' ' && line[i] != '='){
            // Read another character to the name 
            if(readingVarName) get<0>(varStorager) += line[i];
        
        // If it's a separator
        }else if(line[i] == ','){
            // Finish this variable, get ready to read another one
            get<1>(varStorager) = i+1;
            break;
        // If it's the end
        }else if(line[i] == ';'){
            // Finish this variable, end the process (no more vars to read)
            get<1>(varStorager) == -1;
            break;
        }else if(line[i] == '='){
            // End reading the var name; Start reading the initial value
            readingVarName = false;
        }
    }

    // Return the name of the variable
    return varStorager;
}


// Declare variable initializations (initial value)
void declareInitializations(string varInits, int &tabs){
    // To storage the current position
    auto pos = 0;

    // To storage the last position
    int last = 0;

    // Print every init separately
    do{
        // Find where the init begins
        pos = varInits.find("\\", pos + 1);
        // Print tabulations
        printTabs(tabs);
        // Check if this is the last init
        if(pos != string::npos){
            // Print init (from the last place, with a length of 'pos - last')
            cout << varInits.substr(last, (pos - last)) << endl;
            // Define the new last position
            last = pos + 1;
        // If this is the last init, just print the rest of the line
        }else cout << varInits.substr(last) << endl;
    }while(pos != string::npos); // While there are still inits
}


// Declare variables
void declareVariables(string &varType, string &line, int &tabs){
    // Beginning of the declaring statement
    cout << "Definir ";

    // To storage: var name, the pos of the next var (-1 if this is the last), var initial value
    tuple<string, int, string> varStorager("", -1, "");
    // To storage all the variable initializations
    string varInits = "";

    // Skip variable type word
    get<1>(varStorager) = line.find(varType) + varType.size() + 1;

    // Get the variable names
    do{
        // Get next variable name
        varStorager = getVariableName(line, get<1>(varStorager));

        // If it's the last variable
        if(get<1>(varStorager) == -1) cout << get<0>(varStorager) << " ";
        // If there are still more variables
        else cout << get<0>(varStorager) << ", ";

        // If this variable has an initial value
        if(get<2>(varStorager) != ""){
            // Storage the initialization ('\\' means this is the end of the init and the start of the next init)
            if(varInits != "") varInits += "\\";
            varInits += get<0>(varStorager);
            varInits += " =";
            varInits += get<2>(varStorager);
        }
    }while(get<1>(varStorager) != -1); // Repeat until there are no more variables
    
    // Print variable type
    if(varType == "int") cout << "Como Entero" << endl;
    else if(varType == "double" || varType == "float") cout << "Como Real" << endl;
    else if(varType == "string" || varType == "char") cout << "Como Caracter" << endl;
    else if(varType == "boolean") cout << "Como Logico" << endl;

    // Print var initializations (only if there are)
    if(varInits != "") declareInitializations(varInits, tabs);
}


// Declare if statement
pair<int, string> declareIfStatement(string &line, int initialPos){
    // Beginning of the if statement
    cout << "Si ";

    // To storage the full condition
    string condition = "";

    // To count how many parenthesis are still open
    int openPar = 1;

    // To storage the final position
    int finalPos = 0;

    // To mark this if as a one-line if
    string oneLine = "none";

    // Iterate through the whole line
    for(int i=initialPos; i<line.size(); i++){
        // If we open a new parenthesis
        if(line[i] == '(') openPar++;

        // If we close a parenthesis
        if(line[i] == ')'){
            // Close a parenthesis
            openPar--;
            // If there is no more, finish
            if(openPar == 0){
                // Check where the if ends
                for(int j=i+1; j<line.size(); j++){
                    // If it's a space, skip
                    if(line[j] == ' ') continue;
                    // If it's an open bracket, finish
                    else{
                        // Storage the final position (if it's an open bracket, skip it)
                        if(line[j] == '{') finalPos = j+1;
                        else{
                            // If there's a normal character, this is a one-line if
                            oneLine = "if";
                            finalPos = j;
                        }
                        // Finish
                        break;
                    }
                }
                // Finish
                break;
            }
        }

        // Convert the '!' operator to its pseint word
        if(line[i] == '!'){
            condition += "No ";
            continue;
        }

        // Storage the condition as usual
        condition += line[i];
    }

    // End of the if condition part
    cout << condition << " Entonces" << endl;

    // Return the final pos + if this is a one-line if
    return make_pair(finalPos, oneLine);
}


// Declare for statement
pair<int, string> declareForStatement(string &line, int initialPos){
    // Beginning of the for statement
    cout << "Para ";

    // To storage iterator name
    string itName = "";

    // To storage initial value
    string iniValue = "";

    // To storage final value
    string finValue = "";
    // Flag: We're reading the last value
    bool readingFinalVal = false;

    // To storage increaser value
    string incValue = "";

    // To storage the final position
    int finalPos = 0;

    // To mark this if as a one-line for
    string oneLine = "none";

    // Establish the initial position to start checking (skip the 'int')
    initialPos = line.find("int") + 4;
    // Check for the iterator name and initial value
    for(int i=initialPos; i<line.size(); i++){
        // If it's a character different from an equal sign, it's part of the iterator name
        if(line[i] != '='){
            // Add to the iterator name
            itName += line[i];
        // If it's an equal sign, start checking for the initial value
        }else{
            // Check the next part of the for statement until the semicolon
            for(int j=i+1; j<line.size(); j++){
            // If it's a number or a minus sign, it's part of the initial value
                if((line[j] >= '0' && line[j] <= '9') || line[j] == '-'){
                    // Add to the initial value
                    iniValue += line[j];
                // If it's a semicolon, finish
                }else if(line[j] == ';'){
                    // Store the new initial position to continue searching later
                    initialPos = j+1;
                    break;
                }
            }
            break;
        }
    }
    // Print iterator name and initial value
    cout << itName << "<-" << iniValue << " Hasta ";

    // Check for the final value
    for(int i=initialPos; i<line.size(); i++){
        // If we aren't reading the last value
        if(!readingFinalVal){
            // If is a '<' sign
            if(line[i] == '<'){
                // If the next character is a '=' sign, skip it
                if(line[i+1] == '=') i++;
                // If not, add a '-1' to the final value
                else finValue += "-1+";
                // Start reading the final value
                readingFinalVal = true;
            // If is a '>' sign
            }else if(line[i] == '>'){
                // If the next character is a '=' sign, skip it
                if(line[i+1] == '=') i++;
                // If not, add a '+1' to the final value
                else finValue += "1+";
                // Start reading the final value
                readingFinalVal = true;
            }
        }else{
            // If it isn't a semicolon, it's part of the final value
            if(line[i] != ';') finValue += line[i];
            // If it's a semicolon, finish
            }else{
                // Store the new initial position to continue searching later
                initialPos = i+1;
                // Finish
                break;
            }
        }
    }
    // Print final value
    cout << finValue << " Con Paso ";

    // Check for the increment value
    // If it's a "++", the increment value is 1
    // If it's a "--", the increment value is -1
    if((line.find("++", initialPos) != string::npos) || (line.find("--", initialPos) != string::npos)){
        if(line.find("++", initialPos) != string::npos) incValue = "1";
        else if(line.find("--", initialPos) != string::npos) incValue = "-1";

        // Check where the for statement finishes
        for(int i=initialPos; i<line.size(); i++){
            // If it's a close parenthesis, finish
            if(line[i] == ')'){
                // Store the new initial position to continue searching later
                initialPos = i+1;
                break;
            }
        }
    }else{
        // Check for the increment value
        for(int i=initialPos; i<line.size(); i++){
            // If it's a number or a minus sign, it's part of the increment value
            if((line[i] >= '0' && line[i] <= '9') || line[i] == '-'){
                incValue += line[i];
            // If it's a close parenthesis, finish
            }else if(line[i] == ')'){
                // Store the new initial position to continue searching later
                initialPos = i+1;
                break;
            }
        }
    }
    // Print the increment value
    cout << incValue << " Hacer" << endl;

    // Iterate through the rest of the line
    for(int i=initialPos; i<line.size(); i++){
        // If it's a space, skip
        if(line[i] == ' ') continue;
        // If it's other thing, finish
        else{
            // Storage the final position (if it's an open bracket, skip it)
            if(line[i] == '{') finalPos = i+1;
            else{
                // If it's a normal character, this is a one-line for
                oneLine = "for";
                finalPos = i;
            }
            break;
        }
    }

    // Return the final pos + if this is a one-line for
    return make_pair(finalPos, oneLine);
}


// Declare do-while statement
int declareDoWhileStatement(string &line, int initialPos){
    // Beginning of the do-while statement
    cout << "Repetir" << endl;

    // To storage the final position
    int finalPos = 0;

    // Check where the do ends
    for(int i=initialPos; i<line.size(); i++){
        // If it's a space, skip
        if(line[i] == ' ') continue;
        // If it's another character, finish
        else{
            // Move to the current character in the line (if it's an open bracket, skip it)
            if(line[i] == '{') finalPos = i+1;
            else finalPos = i;

            // Finish
            break;
        }
    }

    // Return the final position
    return finalPos;
}


// Declare while statement
int declareWhileStatement(string &line, int initialPos, bool isDoWhile){
    // Beginning of the while statement
    if(!isDoWhile) cout << "Mientras ";
    else cout << "Hasta Que "; 

    // To storage the full condition
    string condition = "";

    // To count how many parenthesis are still open
    int openPar = 1;

    // To storage the final position
    int finalPos = 0;

    // Iterate through the whole line
    for(int i=initialPos; i<line.size(); i++){
        // If we open a new parenthesis
        if(line[i] == '(') openPar++;

        // If we close a parenthesis
        if(line[i] == ')'){
            // Close a parenthesis
            openPar--;
            // If there is no more, finish
            if(openPar == 0){
                // Check where the while ends
                for(int j=i+1; j<line.size(); j++){
                    // If it's a space, skip
                    if(line[j] == ' ') continue;
                    // If it's an open bracket, finish
                    else{
                        // Storage the final position (if it's an open bracket, skip it)
                        if(line[j] == '{') finalPos = j+1;
                        // If there's a normal character, don't skip it
                        else finalPos = j;

                        // Finish
                        break;
                    }
                }

                // Finish
                break;
            }
        }

        // Convert the '!' operator to its pseint word
        if(line[i] == '!'){
            condition += "No ";
            continue;
        }

        // If this is a do/while, we have to invert the condition
        if(isDoWhile){
            // If it's a '<'
            if(line[i] == '<'){
                // If the next character is a '='
                if(line[i+1] == '='){
                    // Invert ('<=' -> '>')
                    condition += ">";
                    // Omit the '='
                    i++;
                // Invert ('<' -> '>=')
                }else condition += ">=";
            // If it's a '>'
            }else if(line[i] == '>'){
                // If the next character is a '='
                if(line[i+1] == '='){
                    // Invert ('>=' -> '<')
                    condition += "<";
                    // Omit the '='
                    i++;
                // Invert ('>' -> '<=')
                }else condition += "<=";
            
            // Storage the condition as usual
            }else condition += line[i];
        // Storage the condition as usual
        }else condition += line[i];
    }

    // End of the while condition part (if it isn't a do/while)
    if(!isDoWhile) cout << condition << " Hacer" << endl;
    else cout << condition << endl;

    // Return the final pos
    return finalPos;
}


// Print output lines
int printOutputLine(int currentChar, string &line){
    // Beginning of the output line
    cout << "Escribir ";

    // To storage the initial position (where the print begins)
    int initialPos = line.find("(", currentChar) + 1;

    // To storage the final position
    int finalPos;

    // To storage the full condition
    string condition = "";

    // To count how many parenthesis are still open
    int openPar = 1;

    // Iterate through the whole line
    for(int i=initialPos; i<line.size(); i++){
        // If we are concatening (also check if it's not an addition)
        if(line[i] == '+' && openPar == 1) condition += ",";
        else{
            // If we open a new parenthesis
            if(line[i] == '(') openPar++;

            // If we close a parenthesis
            if(line[i] == ')'){
                // Close a parenthesis
                openPar--;
                // If there is no more, finish
                if(openPar == 0){
                    // Storage the final position
                    finalPos = i+1;
                    // Finish
                    break;
                }
            }

            // Storage the condition as usual
            condition += line[i];
        }
    }

    // Print full condition
    cout << condition << endl;

    // Return final position
    return finalPos;
}


// Print input lines
int printInputLine(int currentChar, string &line){
    // Beginning of the output line
    cout << "Leer ";

    // To storage the initial position (where the print begins)
    int initialPos = line.find(".next");

    // To storage the final position
    int finalPos = 1;

    // To storage the variable name
    string varName = "";

    // Flag: Reading the variable name
    bool readingVarName = false;

    // Iterate through the whole line (in reverse)
    for(int i=initialPos; i>=0; i--){
        // If we aren't reading the var name
        if(!readingVarName){
            // If it's a '=' sign
            if(line[i] == '='){
                // Start reading the var name
                readingVarName = true;
                // Omit any spaces in between
                for(int j=i-1; j>=0; j--){
                    if(line[j] != ' '){
                        // Continue iterating on this pos
                        i = j+1;
                        break;
                    }
                }
            }
        // If we're reading the var name
        }else{
            // If it's a space or tab, finish
            if(line[i] == ' ' || line[i] == '\t') break;
            // Store the character as usual
            else varName += line[i];
        }
    }

    // Reverse the variable name
    reverse(varName.begin(), varName.end());
    // Print variable name
    cout << varName << endl;

    // To find the final position
    for(int i=initialPos; i<line.size(); i++){
        // If we find a space, finish
        if(line[i] == ' '){
            // Storage this character as the final position
            finalPos = i;
            // Finish
            break;
        }
        // If we reach the end of the line, the final position is the end of the line
        if(i+1 == line.size()) finalPos = line.size();
    }

    // Return final position
    return finalPos;
}


// Print special arithmetic operation
int printSpArithmeticOp(int initialPos, string &line, bool doubleSign){
    // To storage the variable name
    string varName = "";
    // To storage the type of operation we're doing
    char op;
    // To storage the final position
    int finalPos = 0;

    // Iterate through the whole line
    for(int i=initialPos; i<line.size(); i++){
        // If we find an arithmetic sign, storage it and finish reading the var name
        if(line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '%'){
            // Storage the sign
            op = line[i];
            // Omit the sign and the '='
            finalPos = i+2;
            // Finish
            break;
        // Omit spaces and tabs
        }else if(line[i] == ' ' || line[i] == '\t') continue;
        // Add the character to the var name
        else varName += line[i];
    }

    // If it's a double sign operation, print a '1'
    if(doubleSign) cout << varName << " = " << varName << " " << op << " 1" << endl;
    // If it's a special operation, print the rest of the line (omitting the semicolon)
    else{
        cout << varName << " = " << varName << " " << op << " (";
        // If it the rest of the line already has a space, skip it (just for aesthetics)
        if(line[finalPos] == ' ') cout << line.substr(finalPos+1, line.find(";", finalPos) - (finalPos+1));
        else cout << line.substr(finalPos, line.find(";", finalPos) - finalPos);
        cout << ")" << endl;
    }

    // Return the final position
    return line.find(";", finalPos) != string::npos ? line.find(";", finalPos) : line.size();
}


// Print normal lines
void printNormalLine(int currentChar, string &line){
    // Flag: Still tabulations to skip
    bool startEnd = false;

    // Check every character from the current position
    for(int i=currentChar; i<line.size(); i++){
        // If haven't finished skipping the initial tabulations
        if(!startEnd){
            // Check if there are no more tabulations, in that case check the flag
            if(line[i] != '\t' && line[i] != ' ') startEnd = true;
        }

        // If finished skipping the initial tabulations
        if(startEnd){
            // If it's the end of the line, finish
            if(line[i] == ';') break;
            // Else, continue as usual
            else cout << line[i];
        }
    }

    // Print an endl
    cout << endl;
}


// Check if the line is an exclusive java line
bool checkExclusiveLine(int currentChar, string &line){
    // Check if it's a 'package' line
    if(line.find("package ", currentChar) != string::npos) return true;
    // Check if it's an 'import' line
    if(line.find("import ", currentChar) != string::npos) return true;
    // Check if it's a 'Scanner' line
    if(line.find("Scanner ", currentChar) != string::npos) return true;
    // Check if it's a closing thingy
    if(line.find(".close()", currentChar) != string::npos) return true;

    // This isn't an exclusive line
    return false;
}


// -----------------------------------------------------------------------------
// Main
int main(){
    // idk im used to put this thingy
    cin.tie(0) -> sync_with_stdio(); cout.tie(0);

    // Open the code file
    ifstream codeFile("code.txt", ios::in | ios::binary);

    // Check if the file opened correctly
    if(!codeFile.is_open()){
        cerr << "No se pudo abrir el archivo";
        return 1;
    }

    // To storage the current line
    string line;
    // To determine how many tabs to print
    int tabs = 0;
    // To storage the currently open statements
    stack<string> openStatements;


    // Read file (line by line)
    while(getline(codeFile, line)){
        // If line's empty
        if(line.empty()){
            // Print endl and continue
            cout << endl;
            // Omit the line
            continue;
        }
        // Omit the return character at the end of the line
        if(line.back() == '\r') line.pop_back();

        // Flag: This line is empty
        bool emptyLine = true;

        // Check if this line is empty
        for(int i=0; i<line.size(); i++){
            // If is not a space, line break, or tabulation 
            if(line[i] != ' ' && line[i] != '\n' && line[i] != '\t'){
                // This isn't an empty line
                emptyLine = false;
                break;
            }
        }
        // If this is an empty line
        if(emptyLine){
            // Print endl and continue
            cout << endl;
            // Omit the line
            continue;
        }


        // If there's already been a declaration of something, here it storages where we are rn in the line
        int currentChar = 0;
        // To mark we have an open one-line statement (if, else)
        string oneLineStatement = "none";


        // Check if we need to close an open statement
        if(!openStatements.empty() && line.find("}") != string::npos &&
            !(line.find("else") != string::npos && openStatements.top() == "if") &&
            !(line.find("while") != string::npos && openStatements.top() == "dowhile")){
            checkCloseStatement(line, tabs, openStatements, currentChar, false);
        }


        // Check if it's a main function statement declaration
        if(line.find("public static void main(String[] args)") != string::npos){
            // Print tabulations for the new line
            printTabs(tabs);

            // Declare a new main function statement + storage the current character in the line
            cout << "Algoritmo -nombreAqui-" << endl;

            // Check where the if ends
            for(int i=line.find("public static void main(String[] args)") + 38; i<line.size(); i++){
                // If it's a space, skip
                if(line[i] == ' ') continue;
                // If it's an open bracket, finish
                else if(line[i] == '{'){
                    // Move to the current character in the line
                    currentChar = i+1;
                    break;
                }
            }

            // Mark a new open statement
            openStatements.push("main");
            // Add a tabulation
            tabs++;
        }


        // Check if it's an else statement declaration
        if(line.find("else") != string::npos && openStatements.top() == "if"){
            // Print tabulations for the new line
            printTabs(tabs - 1);

            // Declare a new else statement
            cout << "SiNo" << endl;

            // Check where the else ends
            for(int i=line.find("else") + 4; i<line.size(); i++){
                // If it's a space, skip
                if(line[i] == ' ') continue;
                // If it's another character, finish
                else{
                    // Move to the current character in the line (if it's an open bracket, skip it)
                    if(line[i] == '{') currentChar = i+1;
                    else{
                        oneLineStatement = "else";
                        currentChar = i;
                    }
                    break;
                }
            }
        }
        

        // Check if it's an if statement declaration
        if(line.find("if(") != string::npos){
            // Print tabulations for the new line
            printTabs(tabs);

            // Declare a new if statement + storage the current character in the line + storage if it's a one-line if
            pair<int, string> ans = declareIfStatement(line, line.find("if(") + 3);
            currentChar = ans.first;
            oneLineStatement = ans.second;

            // Mark a new open statement
            openStatements.push("if");
            // Add a tabulation
            tabs++;
        }


        // Check if it's a for statement declaration
        if(line.find("for(") != string::npos){
            // Print tabulations for the new line
            printTabs(tabs);

            // Declare a new for statement + storage the current character in the line + storage if it's a one-line for
            pair<int, string> ans = declareForStatement(line, line.find("for(") + 4);
            currentChar = ans.first;
            oneLineStatement = ans.second;

            // Mark a new open statement
            openStatements.push("for");
            // Add a tabulation
            tabs++;
        }


        // Check if it's a do/while statement declaration
        if((line.find("do ") != string::npos) || (line.find("do{") != string::npos)){
            // Print tabulations for the new line
            printTabs(tabs);

            // Declare a new do/while statement + storage the current character in the line
            currentChar = declareDoWhileStatement(line, line.find("do") + 2);

            // Mark a new open statement
            openStatements.push("dowhile");
            // Add a tabulation
            tabs++;
        }


        // Check if it's a while statement declaration
        if(line.find("while(") != string::npos){
            // Check if this is a do/while or a normal while
            if(openStatements.top() == "dowhile"){
                // Print tabulations for the new line (if it's a do/while, print one tab less)
                printTabs(tabs - 1);
                // Declare the finish part of a do/while + storage the current character in the line
                currentChar = declareWhileStatement(line, line.find("while(") + 6, true);
                // Force closing the dowhile
                checkCloseStatement(line, tabs, openStatements, currentChar, true);
            }else{
                // Print tabulations for the new line
                printTabs(tabs);
                // Declare a new while statement + storage the current character in the line
                currentChar = declareWhileStatement(line, line.find("while(") + 6, false);
                // Mark a new open statement
                openStatements.push("while");
                // Add a tabulation
                tabs++;
            }
        }


        // To storage variable types
        string varType = "none";

        // Check if it's a variable declaration (not a function declaration)
        if(line.find("(") == string::npos && line.find("{") == string::npos){
            //cout << line << endl;
            // If you're declaring an integer (Entero)
            if(line.find("int") != string::npos) varType = "int";
            // If you're declaring a double (Real)
            else if(line.find("double") != string::npos) varType = "double";
            // If you're declaring a float (Real)
            else if(line.find("float") != string::npos) varType = "float";
            // If you're declaring a string (Caracter)
            else if(line.find("string") != string::npos) varType = "string";
            // If you're declaring a char (Caracter)
            else if(line.find("char") != string::npos) varType = "char";
            // If you're declaring a boolean (Logico)
            else if(line.find("boolean") != string::npos) varType = "boolean";

            // If you're actually declaring a variable
            if(varType != "none"){
                // Print tabulations for the new line
                printTabs(tabs);

                // Declare a new variable
                declareVariables(varType, line, tabs);

                // Dismiss the rest of the iteration
                continue;
            }
        }
    

        // Check if it's an input line
        if(line.find(".next") != string::npos){
            // Print tabulations for the new line
            printTabs(tabs);

            // Print input line (.next---()) + storage the current character in the line
            currentChar = printInputLine(line.find(".next"), line);
        }


        // Check if it's an output line
        if(line.find("System.out.print") != string::npos){
            // Print tabulations for the new line
            printTabs(tabs);

            // Print output line (system.out.print) + storage the current character in the line
            currentChar = printOutputLine(line.find("System.out.print") + 16, line);
        }


        // Check if it's a special arithmetic operation (+=, -=, *=, /=, %=)
        if(line.find("+=", currentChar) != string::npos ||
            line.find("-=", currentChar) != string::npos ||
            line.find("*=", currentChar) != string::npos ||
            line.find("/=", currentChar) != string::npos ||
            line.find("%=", currentChar) != string::npos){
            // Print tabs
            printTabs(tabs);
            // Print special arithmetic operation
            currentChar = printSpArithmeticOp(currentChar, line, false);
        }


        // Check if it's a special double sign operation (++, --)
        if(line.find("++", currentChar) != string::npos || line.find("--", currentChar) != string::npos){
            // Print tabs
            printTabs(tabs);
            // Print special arithmetic double sign operation
            currentChar = printSpArithmeticOp(currentChar, line, true);
        }

        
        // Check what's the rest of the line
        if(currentChar < line.size() || currentChar == 0){
            // Check if the remaining characters are semicolons or spaces
            for(int i=currentChar; i<line.size(); i++){
                // If it's a normal character, finish
                if(line[i] != ';' && line[i] != ' '){
                    // Storage the new current character
                    currentChar = i;
                    break;
                // If it's the last character + it's a semicolon or space
                }else if(i+1 == line.size()){
                    // This line has nothing else
                    currentChar = line.size();
                }
                // If it's a semicolon or space, skip it
            }

            // Just print the rest of the line (if there's still more)
            if(currentChar < line.size()){
                // Check if it isn't an exclusive java line
                if(!checkExclusiveLine(currentChar, line)){
                    // Print tabulations for the new line
                    printTabs(tabs);

                    // Print normal line
                    printNormalLine(currentChar, line);
                }
            }
        }


        // Check if we need to force a closing one-line if
        if(oneLineStatement == "if"){
            // Storage the current line position
            streampos currPos = codeFile.tellg();
            // To storage the next line
            string nextLine;
            
            // Check if we can read the next line
            if(getline(codeFile, nextLine)){
                // If the next line doesn't have an else statement, force a closing one-line if
                if(nextLine.find("else") == string::npos) checkCloseStatement(line, tabs, openStatements, currentChar, true);
            // If there are no more lines, force a closing one-line if
            }else checkCloseStatement(line, tabs, openStatements, currentChar, true);
            
            // Return the line iterator to the original current line
            codeFile.seekg(currPos);
        }
        // Check if we need to force a closing one-line else
        if(oneLineStatement == "else"){
            checkCloseStatement(line, tabs, openStatements, currentChar, true);
        }
        // Check if we need to force a closing one-line for
        if(oneLineStatement == "for"){
            checkCloseStatement(line, tabs, openStatements, currentChar, true);
        }
    }


    // End of the algorithm
    cout << "FinAlgortimo";

    // Close the code file
    codeFile.close();

    // Haha finish lol
    return 0;
}
// 1043 lines, 26/10/2026