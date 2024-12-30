
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <algorithm>
#include <cctype>
#include <SPIFFS.h>

#define VERBOSE 0
#define PCMODE 0

#if !PCMODE
#include <TFT_eSPI.h> 
#include <SPI.h>
#endif

class Var {
public:
    enum varType {
        v_string,
        v_integer,
        v_boolean,
        v_double,
        v_none
    };
    varType type{ v_none };
    bool valueBoolean;
    std::string valueString;
    int valueInt;
    double valueDouble;
};

class Global {
public:
    Global() {

    }
    std::map<std::string, Var> Vars;
    std::stack<int> lineStack;
    int callGotoReturnLine;
    std::string print;
    std::string error;
    bool repeatCurrentLine;
    bool enableGotoReturnLine;
    bool enableCallLine;
    bool finishProgram;
    int currentLine;
    int color1;
    int color2;
    int x1;
    int y1;
    int x2;
    int y2;
    int par1;
};

class Node {
public:
    std::string name;
    std::string typeNode;
    std::vector<Node*> Nodes;
    Node* linkedNode{ nullptr };
    virtual void eval(Global& global) {};
    Var value;
};

struct AuxExpressionParseNode {
    std::string nodeFunction;
    std::string nodeColapsedName;
    AuxExpressionParseNode* parentNode;
    std::vector<std::string> nodeData;
    std::vector<AuxExpressionParseNode*> childNodes;
    Node* realNode;
};

class EqualTestNode : public Node {
public:

    EqualTestNode() {
        value.type = Var::varType::v_boolean;
        typeNode = "EqualTestNode";
    }

    void eval(Global& global) override {

        Nodes[0]->eval(global);
        Nodes[1]->eval(global);


        Var::varType lastvType = Var::varType::v_none;

        for (auto& node : Nodes) {
            // Check if it is a constant or a var (constant should not have a name)
            if (node->typeNode == "VarNode") {
                if (global.Vars.find(node->name) != global.Vars.end()) {
                    // Load the value from global vars            
                    node->value = global.Vars[node->name];
                }
                else {
                    global.error = "Variable '" + node->name + "' not found.";
                    return;
                }
            }

            if ((lastvType != Var::varType::v_none) && (lastvType != node->value.type)) {
                global.error = "Different data types near to '=='";
                return;
            }
            lastvType = node->value.type;
        }

        switch (Nodes[0]->value.type) {
        case Var::varType::v_string: {
            value.valueBoolean = Nodes[0]->value.valueString == Nodes[1]->value.valueString;
            break;
        }
        case Var::varType::v_boolean: {
            value.valueBoolean = Nodes[0]->value.valueBoolean == Nodes[1]->value.valueBoolean;
            break;
        }
        case Var::varType::v_double: {
            value.valueBoolean = Nodes[0]->value.valueDouble == Nodes[1]->value.valueDouble;
            break;
        }
        case Var::varType::v_integer: {
            value.valueBoolean = Nodes[0]->value.valueInt == Nodes[1]->value.valueInt;
            break;
        }
        }
    }
};



class AssignNode : public Node {
public:

    AssignNode() {
        value.type = Var::varType::v_boolean;
        typeNode = "AssignNode";
    }

    void eval(Global& global) override {
        Nodes[1]->eval(global);
        auto varname = Nodes[0]->name;
        if (global.Vars.find(varname) == global.Vars.end()) {
            global.Vars[varname] = Var();
        }
        global.Vars[varname] = Nodes[1]->value;
    }
};




class InEqualTestNode : public Node {
public:

    InEqualTestNode() {
        value.type = Var::varType::v_boolean;
        typeNode = "InEqualTestNode";
    }

    void eval(Global& global) override {

        Nodes[0]->eval(global);
        Nodes[1]->eval(global);

        Var::varType lastvType = Var::varType::v_none;

        for (auto& node : Nodes) {
            // Check if it is a constant or a var (constant should not have a name)
            if (node->typeNode == "VarNode") {
                if (global.Vars.find(node->name) != global.Vars.end()) {
                    // Load the value from global vars            
                    node->value = global.Vars[node->name];
                }
                else {
                    global.error = "Variable '" + node->name + "' not found.";
                    return;
                }
            }

            if ((lastvType != Var::varType::v_none) && (lastvType != node->value.type)) {
                global.error = "Different data types near to '<>'";
                return;
            }
            lastvType = node->value.type;
        }

        switch (Nodes[0]->value.type) {
        case Var::varType::v_string: {
            value.valueBoolean = Nodes[0]->value.valueString != Nodes[1]->value.valueString;
            break;
        }
        case Var::varType::v_boolean: {
            value.valueBoolean = Nodes[0]->value.valueBoolean != Nodes[1]->value.valueBoolean;
            break;
        }
        case Var::varType::v_double: {
            value.valueBoolean = Nodes[0]->value.valueDouble != Nodes[1]->value.valueDouble;
            break;
        }
        case Var::varType::v_integer: {
            value.valueBoolean = Nodes[0]->value.valueInt != Nodes[1]->value.valueInt;
            break;
        }
        }
    }
};


class GreaterThanTestNode : public Node {
public:

    GreaterThanTestNode() {
        value.type = Var::varType::v_boolean;
        typeNode = "GreaterThanTestNode";
    }

    void eval(Global& global) override {

        Nodes[0]->eval(global);
        Nodes[1]->eval(global);

        Var::varType lastvType = Var::varType::v_none;

        for (auto& node : Nodes) {
            // Check if it is a constant or a var (constant should not have a name)
            if (node->typeNode == "VarNode") {
                if (global.Vars.find(node->name) != global.Vars.end()) {
                    // Load the value from global vars            
                    node->value = global.Vars[node->name];
                }
                else {
                    global.error = "Variable '" + node->name + "' not found.";
                    return;
                }
            }

            if ((lastvType != Var::varType::v_none) && (lastvType != node->value.type)) {
                global.error = "Different data types near to '>'";
                return;
            }
            lastvType = node->value.type;
        }

        switch (Nodes[0]->value.type) {
        case Var::varType::v_string: {
            value.valueBoolean = Nodes[0]->value.valueString > Nodes[1]->value.valueString;
            break;
        }
        case Var::varType::v_boolean: {
            value.valueBoolean = Nodes[0]->value.valueBoolean > Nodes[1]->value.valueBoolean;
            break;
        }
        case Var::varType::v_double: {
            value.valueBoolean = Nodes[0]->value.valueDouble > Nodes[1]->value.valueDouble;
            break;
        }
        case Var::varType::v_integer: {
            value.valueBoolean = Nodes[0]->value.valueInt > Nodes[1]->value.valueInt;
            break;
        }
        }
    }
};

class LessThanTestNode : public Node {
public:

    LessThanTestNode() {
        value.type = Var::varType::v_boolean;
        typeNode = "LessThanTestNode";
    }

    void eval(Global& global) override {

        Nodes[0]->eval(global);
        Nodes[1]->eval(global);

        Var::varType lastvType = Var::varType::v_none;

        for (auto& node : Nodes) {
            // Check if it is a constant or a var (constant should not have a name)
            if (node->typeNode == "VarNode") {
                if (global.Vars.find(node->name) != global.Vars.end()) {
                    // Load the value from global vars            
                    node->value = global.Vars[node->name];
                }
                else {
                    global.error = "Variable '" + node->name + "' not found.";
                    return;
                }
            }

            if ((lastvType != Var::varType::v_none) && (lastvType != node->value.type)) {
                global.error = "Different data types near to '<'";
                return;
            }
            lastvType = node->value.type;
        }

        switch (Nodes[0]->value.type) {
        case Var::varType::v_string: {
            value.valueBoolean = Nodes[0]->value.valueString < Nodes[1]->value.valueString;
            break;
        }
        case Var::varType::v_boolean: {
            value.valueBoolean = Nodes[0]->value.valueBoolean < Nodes[1]->value.valueBoolean;
            break;
        }
        case Var::varType::v_double: {
            value.valueBoolean = Nodes[0]->value.valueDouble < Nodes[1]->value.valueDouble;
            break;
        }
        case Var::varType::v_integer: {
            value.valueBoolean = Nodes[0]->value.valueInt < Nodes[1]->value.valueInt;
            break;
        }
        }
    }
};


class ExpressionNode : public Node {

public:
    ExpressionNode() {
        typeNode = "ExpressionNode";
    }
    void eval(Global& global) override {

        Nodes[0]->eval(global);
        switch (Nodes[0]->value.type) {
        case Var::varType::v_string: {
            value.valueString = Nodes[0]->value.valueString;
            break;
        }
        case Var::varType::v_boolean: {
            value.valueBoolean = Nodes[0]->value.valueBoolean;
            break;
        }
        case Var::varType::v_double: {
            value.valueDouble = Nodes[0]->value.valueDouble;
            break;
        }
        case Var::varType::v_integer: {
            value.valueInt = Nodes[0]->value.valueInt;
            break;
        }
        }
        value.type = Nodes[0]->value.type;
    }
};


class IFNode : public Node {
private:
    bool Condition(Global& global) {
        bool ret = false;
        Nodes[0]->eval(global);
        switch (Nodes[0]->value.type) {
        case Var::varType::v_string: {
            if (Nodes[0]->value.valueString != "")
                ret = true;
            break;
        }
        case Var::varType::v_boolean: {
            ret = Nodes[0]->value.valueBoolean;
            break;
        }
        case Var::varType::v_double: {
            if (Nodes[0]->value.valueDouble != 0.0)
                ret = true;
            break;
        }
        case Var::varType::v_integer: {
            if (Nodes[0]->value.valueInt != 0)
                ret = true;
            break;
        }
        }
        return ret;
    }

    void Then(Global& global) {
        Nodes[1]->eval(global);
    }

    void Else(Global& global) {
        Nodes[2]->eval(global);
    }
public:
    IFNode()
    {
        typeNode = "IFNode";
    }

    void AddNodes(Node* Condition, Node* Then, Node* Else) {
        Nodes.push_back(Condition);
        Nodes.push_back(Then);
        Nodes.push_back(Else);
    }

    void eval(Global& global) override {
        if (Condition(global))
            Then(global);
        else
            Else(global);
    }
};

class THENNode : public Node {

public:
    THENNode() {
        typeNode = "THENNode";
    }

    void eval(Global& global) override {

    }
};

class ELSENode : public Node {

public:
    ELSENode() {
        typeNode = "ELSENode";
    }

    void eval(Global& global) override {

    }
};


class FORNode : public Node {

public:
    FORNode()
    {
        typeNode = "FORNode";
    }
    //FOR I = 1 TO 5 <action> NEXT
    // FOR I = 10 TO 0 <action> STEP -1
    //FOR <var init> TO <var finish> <action> NEXT/STEP <step value>
    void AddNodes(Node* varInit, Node* varFinish, Node* action, Node* nextStep) {
        Nodes.push_back(varInit);
        Nodes.push_back(varFinish);
        Nodes.push_back(action);
        Nodes.push_back(nextStep);
    }

    void eval(Global& global) override {

        // Standard is repeat the current line
        global.repeatCurrentLine = true;

        // Check if the var was initiated
        if (global.Vars.find(Nodes[0]->name) != global.Vars.end()) {

            // Already initiated, so proccess the increase or decrease
            global.Vars[Nodes[0]->name].valueInt += Nodes[3]->value.valueInt;

            // run the action
            Nodes[2]->eval(global);

            // Check the limit to finish the loop
            if (global.Vars[Nodes[0]->name].valueInt == Nodes[1]->value.valueInt) {
                // Remove the auxiliary variable 
                global.Vars.erase(Nodes[0]->name);

                // Enable load the next line instead repeat this one
                global.repeatCurrentLine = false;
            }
        }
        else {
            // Not initiated, so initiate
            global.Vars[Nodes[0]->name] = Nodes[0]->value;

            // run the action
            Nodes[2]->eval(global);
        }
    }
};



//WHILE y < 5 : x = x+1
class WHILENode : public Node {
private:
    bool Condition(Global& global) {
        bool ret = false;
        Nodes[0]->eval(global);
        switch (Nodes[0]->value.type) {
        case Var::varType::v_string: {
            if (Nodes[0]->value.valueString != "")
                ret = true;
            break;
        }
        case Var::varType::v_boolean: {
            ret = Nodes[0]->value.valueBoolean;
            break;
        }
        case Var::varType::v_double: {
            if (Nodes[0]->value.valueDouble != 0.0)
                ret = true;
            break;
        }
        case Var::varType::v_integer: {
            if (Nodes[0]->value.valueInt != 0)
                ret = true;
            break;
        }
        }
        return ret;
    }
public:
    WHILENode()
    {
        typeNode = "WHILENode";
    }
    void AddNodes(Node* condition, Node* action) {
        Nodes.push_back(condition);
        Nodes.push_back(action);
    }

    void eval(Global& global) override {
        // Standard is repeat the current line
        global.repeatCurrentLine = true;

        if (Condition(global)) {
            Nodes[1]->eval(global);
        }
        else {
            global.repeatCurrentLine = false;
        }
    }
};


//WHILE...WEND

class ANDNode : public Node {
public:
    ANDNode() {
        typeNode = "ANDNode";
    }

    void eval(Global& global) override {
        value.type = Var::varType::v_boolean;
        value.valueBoolean = true;
        for (auto& node : Nodes) {
            node->eval(global);
            if (node->value.valueBoolean == false) {
                value.valueBoolean = false;
                return;
            }
        }
    }
};

class ORNode : public Node {
public:
    ORNode() {
        typeNode = "ORNode";
    }

    void eval(Global& global) override {
        value.valueBoolean = false;
        value.type = Var::varType::v_boolean;
        for (auto& node : Nodes) {
            node->eval(global);
            if (node->value.valueBoolean == true) {
                value.valueBoolean = true;
                return;
            }
        }
    }
};

class AddNode : public Node {

public:
    AddNode() {
        typeNode = "AddNode";
    }

    void eval(Global& global) override {

        Var::varType lastvType = Var::varType::v_none;

        for (auto& node : Nodes) {
            // Check if it is a constant or a var (constant should not have a name)
            if (node->typeNode == "VarNode") {
                if (global.Vars.find(node->name) != global.Vars.end()) {
                    // Load the value from global vars            
                    node->value = global.Vars[node->name];
                }
                else {
                    global.error = "Variable '" + node->name + "' was not declared.";
                    return;
                }
            }

            if ((lastvType != Var::varType::v_none) && (lastvType != node->value.type)) {
                global.error = "Different data types near to '+'";
                return;
            }
            lastvType = node->value.type;
        }

        value.valueBoolean = false;
        value.valueDouble = 0.0;
        value.valueInt = 0;
        value.valueString = "";

        for (auto& node : Nodes) {
            switch (lastvType) {
            case Var::varType::v_string: {
                value.valueString += node->value.valueString;
                value.type = Var::varType::v_string;
                break;
            }
            case Var::varType::v_boolean: {
                value.valueBoolean += node->value.valueBoolean;
                value.type = Var::varType::v_boolean;
                break;
            }
            case Var::varType::v_double: {
                value.valueDouble += node->value.valueDouble;
                value.type = Var::varType::v_double;
                break;
            }
            case Var::varType::v_integer: {
                value.valueInt += node->value.valueInt;
                value.type = Var::varType::v_integer;
                break;
            }
            }
        }
    }
};

class SubNode : public Node {

public:
    SubNode() {
        typeNode = "SubNode";
    }

    void eval(Global& global) override {

        Var::varType lastvType = Var::varType::v_none;

        for (auto& node : Nodes) {
            // Check if it is a constant or a var (constant should not have a name)
            if (node->typeNode == "VarNode") {
                if (global.Vars.find(node->name) != global.Vars.end()) {
                    // Load the value from global vars            
                    node->value = global.Vars[node->name];
                }
                else {
                    global.error = "Variable '" + node->name + "' was not declared.";
                    return;
                }
            }

            if ((lastvType != Var::varType::v_none) && (lastvType != node->value.type)) {
                global.error = "Different data types near to '-'";
                return;
            }
            lastvType = node->value.type;
        }

        value.valueBoolean = false;
        value.valueDouble = 0.0;
        value.valueInt = 0;
        value.valueString = "";

        bool first = true;
        for (auto& node : Nodes) {
            switch (lastvType) {
            case Var::varType::v_string: {
                global.error = "The '-' is not allowed for strings";
                value.type = Var::varType::v_string;
                return;
            }
            case Var::varType::v_boolean: {
                value.type = Var::varType::v_boolean;
                if (first) {
                    value.valueBoolean = node->value.valueBoolean;
                    first = false;
                }
                else {
                    value.valueBoolean -= node->value.valueBoolean;
                }
                break;
            }
            case Var::varType::v_double: {
                value.type = Var::varType::v_double;
                if (first) {
                    value.valueDouble = node->value.valueDouble;
                    first = false;
                }
                else {
                    value.valueDouble -= node->value.valueDouble;
                }
                break;
            }
            case Var::varType::v_integer: {
                value.type = Var::varType::v_integer;
                if (first) {
                    first = false;
                    value.valueInt = node->value.valueInt;
                }
                else {
                    value.valueInt -= node->value.valueInt;
                }
                break;
            }
            }
        }
    }
};

class MulNode : public Node {
public:
    MulNode() {
        typeNode = "MulNode";
    }

    void eval(Global& global) override {
        Var::varType lastvType = Var::varType::v_none;

        for (auto& node : Nodes) {
            // Check if it is a constant or a var (constant should not have a name)
            if (node->typeNode == "VarNode") {
                if (global.Vars.find(node->name) != global.Vars.end()) {
                    // Load the value from global vars            
                    node->value = global.Vars[node->name];
                }
                else {
                    global.error = "Variable '" + node->name + "' was not declared.";
                    return;
                }
            }

            if ((lastvType != Var::varType::v_none) && (lastvType != node->value.type)) {
                global.error = "Different data types near to '-'";
                return;
            }
            lastvType = node->value.type;
        }

        value.valueBoolean = false;
        value.valueDouble = 0.0;
        value.valueInt = 0;
        value.valueString = "";

        bool first = true;
        for (auto& node : Nodes) {
            switch (lastvType) {
            case Var::varType::v_string: {
                global.error = "The '*' is not allowed for strings";
                value.type = Var::varType::v_string;
                return;
            }
            case Var::varType::v_boolean: {
                value.type = Var::varType::v_boolean;
                if (first) {
                    value.valueBoolean = node->value.valueBoolean;
                    first = false;
                }
                else {
                    value.valueBoolean *= node->value.valueBoolean;
                }
                break;
            }
            case Var::varType::v_double: {
                value.type = Var::varType::v_double;
                if (first) {
                    value.valueDouble = node->value.valueDouble;
                    first = false;
                }
                else {
                    value.valueDouble *= node->value.valueDouble;
                }
                break;
            }
            case Var::varType::v_integer: {
                value.type = Var::varType::v_integer;
                if (first) {
                    first = false;
                    value.valueInt = node->value.valueInt;
                }
                else {
                    value.valueInt *= node->value.valueInt;
                }
                break;
            }
            }
        }
    }
};

class DivNode : public Node {
public:
    DivNode() {
        typeNode = "DivNode";
    }

    void eval(Global& global) override {
        Var::varType lastvType = Var::varType::v_none;

        for (auto& node : Nodes) {
            // Check if it is a constant or a var (constant should not have a name)
            if (node->typeNode == "VarNode") {
                if (global.Vars.find(node->name) != global.Vars.end()) {
                    // Load the value from global vars            
                    node->value = global.Vars[node->name];
                }
                else {
                    global.error = "Variable '" + node->name + "' not found.";
                    return;
                }
            }

            if ((lastvType != Var::varType::v_none) && (lastvType != node->value.type)) {
                global.error = "Different data types near to '-'";
                return;
            }
            lastvType = node->value.type;
        }

        value.valueBoolean = false;
        value.valueDouble = 0.0;
        value.valueInt = 0;
        value.valueString = "";

        bool first = true;
        for (auto& node : Nodes) {
            switch (lastvType) {
            case Var::varType::v_string: {
                value.type = Var::varType::v_string;
                global.error = "The '/' is not allowed for strings";
                return;
            }
            case Var::varType::v_boolean: {
                value.type = Var::varType::v_boolean;
                if (first) {
                    value.valueBoolean = node->value.valueBoolean;
                    first = false;
                }
                else {
                    if (node->value.valueBoolean == false) {
                        global.error = "Division by zero";
                        return;
                    }
                    value.valueBoolean /= node->value.valueBoolean;
                }
                break;
            }
            case Var::varType::v_double: {
                value.type = Var::varType::v_double;
                if (first) {
                    value.valueDouble = node->value.valueDouble;
                    first = false;
                }
                else {
                    if (node->value.valueDouble == 0.0) {
                        global.error = "Division by zero";
                        return;
                    }
                    value.valueDouble /= node->value.valueDouble;
                }
                break;
            }
            case Var::varType::v_integer: {
                value.type = Var::varType::v_integer;
                if (first) {
                    first = false;
                    value.valueInt = node->value.valueInt;
                }
                else {
                    if (node->value.valueInt == 0) {
                        global.error = "Division by zero";
                        return;
                    }
                    value.valueInt /= node->value.valueInt;
                }
                break;
            }
            }
        }
    }
};

class ConstStringNode : public Node {
public:
    ConstStringNode() {
        value.type = Var::varType::v_string;
        typeNode = "ConstStringNode";
    }
};

class ConstIntNode : public Node {
public:
    ConstIntNode() {
        value.type = Var::varType::v_integer;
        typeNode = "ConstIntNode";
    }
};

class ConstFloatNode : public Node {
public:
    ConstFloatNode() {
        value.type = Var::varType::v_double;
        typeNode = "ConstFloatNode";
    }
};

class ConstBoolNode : public Node {
public:
    ConstBoolNode() {
        value.type = Var::varType::v_boolean;
        typeNode = "ConstBoolNode";
    }
};

class VarNode : public Node {
public:

    VarNode() {
        typeNode = "VarNode";
    }

    // read operation
    void eval(Global& global) override {
        if (global.Vars.find(name) != global.Vars.end()) {
            value.type = global.Vars[name].type;

            switch (value.type) {
            case Var::varType::v_string: {
                value.valueString = global.Vars[name].valueString;
                break;
            }
            case Var::varType::v_boolean: {
                value.valueBoolean = global.Vars[name].valueBoolean;
                break;
            }
            case Var::varType::v_double: {
                value.valueDouble = global.Vars[name].valueDouble;
                break;
            }
            case Var::varType::v_integer: {
                value.valueInt = global.Vars[name].valueInt;
                break;
            }
            }
        }

       

    };
};

class FncPrintNode : public Node {
public:
    FncPrintNode() {
        typeNode = "FncPrintNode";
    }
    void eval(Global& global) override {
        Node* varNode = this;
        if (!varNode->linkedNode) return;
        varNode = varNode->linkedNode;
        bool finish = false;
        while (!finish) {

            if (varNode->typeNode == "FncPrintNode") {
                global.print += varNode->value.valueString;
            }
            else if (varNode->typeNode == "ExpressionNode") {
                varNode->eval(global);
                switch (varNode->value.type) {
                case Var::varType::v_string: {
                    global.print += varNode->value.valueString;
                    break;
                }
                case Var::varType::v_boolean: {
                    if (varNode->value.valueBoolean) {
                        global.print += "TRUE";
                    }
                    else {
                        global.print += "FALSE";
                    }
                    break;
                }
                case Var::varType::v_double: {
                    global.print += std::to_string(varNode->value.valueDouble);
                    break;
                }
                case Var::varType::v_integer: {
                    global.print += std::to_string(varNode->value.valueInt);
                    break;
                }
                }
            }
            if (varNode->linkedNode) {
                varNode = varNode->linkedNode;
            }
            else {
                finish = true;
            }
        }
    }
};

class GotoNode : public Node {
public:
    GotoNode() {
        typeNode = "GotoNode";
    }

    void eval(Global& global) override {
        global.callGotoReturnLine = value.valueInt;
        global.enableGotoReturnLine = true;
    }
};

class GosubNode : public Node {
public:
    GosubNode() {
        typeNode = "GosubNode";
    }
    void eval(Global& global) override {
        global.callGotoReturnLine = value.valueInt;
        global.enableCallLine = true;
    }
};

class ReturnNode : public Node {
public:
    ReturnNode() {
        typeNode = "ReturnNode";
    }
    void eval(Global& global) override {
        if (!global.lineStack.empty()) {
            global.callGotoReturnLine = global.lineStack.top();
            global.lineStack.pop();
            global.enableGotoReturnLine = true;
        }
        else {
            global.finishProgram = true;
        }
    }
};

// 20 COLOR 14, 1 
//30 PRINT "Texto amarelo em fundo azul."
class ColorNode : public Node {
public:
    ColorNode() {
        typeNode = "ColorNode";
    }
    void eval(Global& global) override {
        Node* node = linkedNode;
        if (node) {
            node->eval(global);
            global.color1 = node->value.valueInt;
            node = node->linkedNode;
            if (node) {
                node->eval(global);
                global.color2 = node->value.valueInt;
            }
        }
    }
};

//20 LOCATE 5, 10 
//30 PRINT "Texto na posição (5, 10)"
class LocateNode : public Node {
public:
    LocateNode() {
        typeNode = "LocateNode";
    }
    void eval(Global& global) override {
        Node* node = linkedNode;
        if (node) {
            node->eval(global);
            global.x1 = node->value.valueInt;
            node = node->linkedNode;
            if (node) {
                node->eval(global);
                global.y1 = node->value.valueInt;
            }
        }
    }
};

// CIRCLE(50, 50), 25, 14
class CircleNode : public Node {
public:
    CircleNode() {
        typeNode = "CircleNode";
    }
    void eval(Global& global) override {
        Node* node = linkedNode;
        if (node) {
            node->eval(global);
            global.x1 = node->value.valueInt;
            node = node->linkedNode;
            if (node) {
                node->eval(global);
                global.y1 = node->value.valueInt;
                node = node->linkedNode;
                if (node) {
                    node->eval(global);
                    global.x2 = node->value.valueInt;
                    node = node->linkedNode;
                    if (node) {
                        node->eval(global);
                        global.color1 = node->value.valueInt;
                    }
                }
            }
        }
    }
};

//LINE(10, 10) - (100, 100), 15
class LineNode : public Node {
public:
    LineNode() {
        typeNode = "LineNode";
    }
    void eval(Global& global) override {
        Node* node = linkedNode;
        if (node) {
            node->eval(global);
            global.x1 = node->value.valueInt;
            node = node->linkedNode;
            if (node) {
                node->eval(global);
                global.y1 = node->value.valueInt;
                node = node->linkedNode;
                if (node) {
                    node->eval(global);
                    global.x2 = node->value.valueInt;
                    node = node->linkedNode;
                    if (node) {
                        node->eval(global);
                        global.y2 = node->value.valueInt;
                        node = node->linkedNode;
                        if (node) {
                            node->eval(global);
                            global.color1 = node->value.valueInt;
                        }
                    }
                }
            }
        }
    }
};

class SleepNode : public Node {
public:
    SleepNode() {
        typeNode = "SleepNode";
    }
    void eval(Global& global) override {        
        Node* node = linkedNode;        
        if (node) {            
            node->eval(global);            
            global.par1 = node->value.valueInt;            
        }
    }
};

class EndNode : public Node {
public:
    EndNode() {
        typeNode = "EndNode";
    }
    void eval(Global& global) override {
        global.finishProgram = true;
    }
};

class ClsNode : public Node {
public:
    ClsNode() {
        typeNode = "ClsNode";
    }
    void eval(Global& global) override {
    }
};

class LineOps {
private:
    std::vector<Node*> Ops;
    Node* FirstNodeOp_;

public:

    LineOps() : FirstNodeOp_(nullptr) {

    }

    ~LineOps() {
      for (const auto& node : Ops) {
          delete node;
      }
      Ops.clear();
    }

    void SetFirstOp(Node* NodeOp) {
        FirstNodeOp_ = NodeOp;
    }

    Node* CreateNode() {
        Ops.push_back(new Node());
        Node* node = (Node*)(Ops.back());
        return node;
    }

    GotoNode* CreateGotoNode() {
        Ops.push_back(new GotoNode());
        GotoNode* node = (GotoNode*)(Ops.back());
        return node;
    }

    GosubNode* CreateGosubNode() {
        Ops.push_back(new GosubNode());
        GosubNode* node = (GosubNode*)(Ops.back());
        return node;
    }

    ReturnNode* CreateReturnNode() {
        Ops.push_back(new ReturnNode());
        ReturnNode* node = (ReturnNode*)(Ops.back());
        return node;
    }

    IFNode* CreateIF() {
        Ops.push_back(new IFNode());
        IFNode* node = (IFNode*)(Ops.back());
        return node;
    }

    FORNode* CreateFOR() {
        Ops.push_back(new FORNode());
        FORNode* node = (FORNode*)(Ops.back());
        return node;
    }

    WHILENode* CreateWHILE() {
        Ops.push_back(new WHILENode());
        WHILENode* node = (WHILENode*)(Ops.back());
        return node;
    }

    EqualTestNode* CreateEqual() {
        Ops.push_back(new EqualTestNode());
        EqualTestNode* node = (EqualTestNode*)(Ops.back());
        return node;
    }

    AssignNode* CreateAssign() {
        Ops.push_back(new AssignNode());
        AssignNode* node = (AssignNode*)(Ops.back());
        return node;
    }

    InEqualTestNode* CreateInEqual() {
        Ops.push_back(new InEqualTestNode());
        InEqualTestNode* node = (InEqualTestNode*)(Ops.back());
        return node;
    }

    GreaterThanTestNode* CreateGreaterThan() {
        Ops.push_back(new GreaterThanTestNode());
        GreaterThanTestNode* node = (GreaterThanTestNode*)(Ops.back());
        return node;
    }

    LessThanTestNode* CreateLessThan() {
        Ops.push_back(new LessThanTestNode());
        LessThanTestNode* node = (LessThanTestNode*)(Ops.back());
        return node;
    }

    ANDNode* CreateAND() {
        Ops.push_back(new ANDNode());
        ANDNode* node = (ANDNode*)(Ops.back());
        return node;
    }

    ORNode* CreateOR() {
        Ops.push_back(new ORNode());
        ORNode* node = (ORNode*)(Ops.back());
        return node;
    }

    DivNode* CreateDIV() {
        Ops.push_back(new DivNode());
        DivNode* node = (DivNode*)(Ops.back());
        return node;
    }

    MulNode* CreateMUL() {
        Ops.push_back(new MulNode());
        MulNode* node = (MulNode*)(Ops.back());
        return node;
    }

    SubNode* CreateSUB() {
        Ops.push_back(new SubNode());
        SubNode* node = (SubNode*)(Ops.back());
        return node;
    }

    AddNode* CreateADD() {
        Ops.push_back(new AddNode());
        AddNode* node = (AddNode*)(Ops.back());
        return node;
    }

    ExpressionNode* CreateExpression() {
        Ops.push_back(new ExpressionNode());
        ExpressionNode* node = (ExpressionNode*)(Ops.back());
        return node;
    }

    ConstIntNode* CreateConstInt() {
        Ops.push_back(new ConstIntNode());
        ConstIntNode* node = (ConstIntNode*)(Ops.back());
        return node;
    }

    ConstStringNode* CreateConstString() {
        Ops.push_back(new ConstStringNode());
        ConstStringNode* node = (ConstStringNode*)(Ops.back());
        return node;
    }

    ConstBoolNode* CreateConstBool() {
        Ops.push_back(new ConstBoolNode());
        ConstBoolNode* node = (ConstBoolNode*)(Ops.back());
        return node;
    }

    ConstFloatNode* CreateConstFloat() {
        Ops.push_back(new ConstFloatNode());
        ConstFloatNode* node = (ConstFloatNode*)(Ops.back());
        return node;
    }

    VarNode* CreateVar() {
        Ops.push_back(new VarNode());
        VarNode* node = (VarNode*)(Ops.back());
        return node;
    }

    FncPrintNode* CreatePrint() {
        Ops.push_back(new FncPrintNode());
        FncPrintNode* node = (FncPrintNode*)(Ops.back());
        return node;
    }

    ColorNode* CreateColor() {
        Ops.push_back(new ColorNode());
        ColorNode* node = (ColorNode*)(Ops.back());
        return node;
    }

    LocateNode* CreateLocate() {
        Ops.push_back(new LocateNode());
        LocateNode* node = (LocateNode*)(Ops.back());
        return node;
    }

    CircleNode* CreateCircle() {
        Ops.push_back(new CircleNode());
        CircleNode* node = (CircleNode*)(Ops.back());
        return node;
    }

    LineNode* CreateLine() {
        Ops.push_back(new LineNode());
        LineNode* node = (LineNode*)(Ops.back());
        return node;
    }

    SleepNode* CreateSleep() {
        Ops.push_back(new SleepNode());
        SleepNode* node = (SleepNode*)(Ops.back());
        return node;
    }

    EndNode* CreateEnd() {
        Ops.push_back(new EndNode());
        EndNode* node = (EndNode*)(Ops.back());
        return node;
    }
    ClsNode* CreateCls() {
        Ops.push_back(new ClsNode());
        ClsNode* node = (ClsNode*)(Ops.back());
        return node;
    }
};

#if !PCMODE
TFT_eSPI tft = TFT_eSPI();    
#endif

std::string inputString = "";
bool stringComplete = false;
bool newCharReceived = false;

Node* ParseLine(std::vector<std::string>::iterator token, std::vector<std::string>::iterator tokensEnd, LineOps& lineOps);

std::string ProgStr = "";

#if 0
std::string ProgStr =
"10 CLS \n"
"12 LOCATE 0,0 \n"
"15 print \"Start\" \n"
"20 x=0 \n"
"30 y=20 \n"
"40 print y + 1 \n"
"50 print \"x:\";x \n"
"60 WHILE x <> 10 : GOSUB 200 WEND\n"
"70 print \"poswhile\" \n"
"80 sleep 2000 \n"
"90 goto 10 \n"
"200 print \"x in while\" ; x \n"
"210 x = x + 1 \n"
"212 z = x * 20 \n"
"213 LINE 0,0,320,z,x\n"
"215 sleep 100 \n"
"230 return \n"
;
#endif

#if 0
std::string ProgStr =
"80 LINE 10,40,120,500,9\n"
"90 CIRCLE 100,150,20,5\n"
"100 COLOR 1,4\n"
"200 y = 1 \n"
"300 x = 2 \n"
"1000 WHILE x <> 5 : GOSUB 10000 WEND\n"
"3000 z = 20 \n"
"3500 PRINT \"z: \" ; z ; \" x: \" ; x  \n"
"3501 LOCATE 150,150 \n"
"3502 PRINT \"Teste\" \n"
"3503 count = 0 \n"
"3504  WHILE count <> 500 : GOSUB 8000 WEND\n"
"3505 COLOR 3,2 \n"
"3508 PRINT \"FIM\" \n"
"3509 SLEEP 3000 \n"
"3510 CLS \n"
"3520 RETURN \n"
"8000 count = count + 10 \n"
"8002 SLEEP 500 \n"
"8005 LINE 10,40,count,500,5\n"
"8010 RETURN \n"
"4000 RETURN \n"
"10000 x = x+1 \n"
"11000 y = x*2 \n"
"12000 RETURN \n"
;
#endif


#if 0
std::string ProgStr =
"80 LINE 10,40,120,500,9\n"
"81 LOCATE 50,50 \n"
"81 PRINT \"Init\" \n"
"90 CIRCLE 100,150,20,5\n"
"100 COLOR 1,3\n"
"200 y = 1 \n"
"300 x = 2 \n"
"301 LOCATE 60,50 \n"
"302 PRINT \"PREWHILE\" \n"
"1000 WHILE x <> 5 : GOSUB 10000 WEND\n"
"1001 LOCATE 90,50 \n"
"1002 PRINT \"POSWHILE\" \n"
"3000 z = 20 \n"
"3500 PRINT \"z: \" ; z ; \" x: \" ; x  \n"
"3501 LOCATE 150,150 \n"
"3502 PRINT \"Teste\" \n"
"3503 count = 0 \n"
"3504  WHILE count <> 500 : GOSUB 8000 WEND\n"
"3505 COLOR 3,2 \n"
"3508 PRINT \"FIM\" \n"
"3509 SLEEP 3000 \n"
"3510 CLS \n"
"3520 GOTO 80 \n"
"8000 count = count + 10 \n"
"8002 SLEEP 500 \n"
"8005 LINE 10,40,count,500,5\n"
"8010 RETURN \n"
"4000 RETURN \n"
"10000 x = x+1 \n"
"10001 LOCATE 90,100 \n"
"10002 PRINT x \n"
"11000 y = x*2 \n"
"12000 RETURN \n"
;
#endif

std::map<int, std::vector<std::string>> program;

bool charIsNum(char c) {
    if (c < 48 || c > 57)
        return false;
    return true;
}

bool isInteger(const std::string& str) {
    try {
        size_t pos;
        std::stoi(str, &pos);
        return pos == str.size();
    }
    catch (const std::invalid_argument&) {
        return false;
    }
    catch (const std::out_of_range&) {
        return false;
    }
}

bool isFloat(const std::string& str) {
    try {
        size_t pos;
        std::stof(str, &pos);
        return pos == str.size();
    }
    catch (const std::invalid_argument&) {
        return false;
    }
    catch (const std::out_of_range&) {
        return false;
    }
}

bool isBoolean(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr == "true" || lowerStr == "false";
}

std::vector<std::string> Tokenize(std::string line)
{
    std::vector<std::string> tokens;
    std::string token;
    bool stringOn = false;
    bool diffCheckOn = false;
    for (auto& c : line) {
        if (stringOn == false && (c == ' ' || c == '\t')) {
            if (!token.empty()) {
                std::pair<std::string, size_t> t;
                tokens.push_back(token);
                token.clear();
            }
        }
        else if (c == '\"') {
            if (stringOn) {
                tokens.push_back("\"" + token + "\"");
                token.clear();
            }
            stringOn = !stringOn;
        }
        else if (stringOn) {
            token = token + c;
            continue;
        }
        else if (diffCheckOn == false && c == '<') {
            diffCheckOn = true;
        }
        else if (diffCheckOn == true && c == '>') {
            diffCheckOn = false;
            tokens.push_back("<>");
        }
        else if (c == '[' || c == ']' || c == ',' || c == ';' || c == ':' || c == '/'
            || c == '=' || c == '==' || c == '>' || c == '<' || c == '!' || c == '\\' ||
            c == '+' || c == '-' || c == '*' || c == '@' || c == '(' || c == ')') {
            if (!token.empty()) {
                std::pair<std::string, size_t> t;
                tokens.push_back(token);
                token = "";
            }
            token = c;
            tokens.push_back(token);
            token = "";
        }
        else {
            token = token + c;
        }
    }

    if (!token.empty()) {
        std::pair<std::string, size_t> t;
        tokens.push_back(token);
    }
    return tokens;
}

std::stack<std::vector<AuxExpressionParseNode*>> BFSAuxExpressionParse(AuxExpressionParseNode* startAuxNode) {
    std::stack<std::vector<AuxExpressionParseNode*>> nodesStackLayer;
    AuxExpressionParseNode* pAuxNode = startAuxNode;
    std::vector<AuxExpressionParseNode*> newLayer = std::vector<AuxExpressionParseNode*>();
    newLayer.push_back(pAuxNode);
    nodesStackLayer.push(newLayer);
    while (true) {
        std::vector<AuxExpressionParseNode*> layer = nodesStackLayer.top();
        std::vector<AuxExpressionParseNode*> newLayer = std::vector<AuxExpressionParseNode*>();
        for (AuxExpressionParseNode* node : layer) {
            for (AuxExpressionParseNode* childNode : node->childNodes) {
                newLayer.push_back(childNode);
            }
        }
        if (!newLayer.empty()) {
            nodesStackLayer.push(newLayer);
        }
        else {
            break;
        }
    }
    return nodesStackLayer;
}

void colapseParentNodeData(std::vector<std::string>& parent, std::vector<std::string>& child, std::string& childColapsedName) {
    size_t posColapse = 0;
    bool colapsePositionFound = false;
    for (size_t i = 0; i <= parent.size() - child.size(); ++i) {
        if (std::equal(child.begin(), child.end(), parent.begin() + i)) {
            posColapse = i;
            colapsePositionFound = true;
            break;
        }
    }
    if (colapsePositionFound) {
        childColapsedName = "";
        for (auto& str : child) {
            childColapsedName += "|" + str;
        }
        childColapsedName += "|";
        parent[posColapse] = childColapsedName;
        parent.erase(parent.begin() + posColapse + 1, parent.begin() + posColapse + child.size());
        if (posColapse > 0 && posColapse < parent.size()) {
            if (parent[posColapse - 1] == "(" && parent[posColapse + 1] == ")") {
                parent.erase(parent.begin() + posColapse + 1);
                parent.erase(parent.begin() + posColapse - 1);
            }
        }
    }
}

void ParseEXPRESSION(ExpressionNode* expression, std::vector<std::string>::iterator token, std::vector<std::string>::iterator tokensEnd, LineOps& lineOps, bool isCondition)
{
    std::vector<std::string>::iterator fromtoken = token;
    std::vector<AuxExpressionParseNode*> nodesToDelete;
    int sts = 0;
    int countParentesis = 0;
    std::vector<std::string> strVect;
    std::string strColapsed;
    AuxExpressionParseNode* pstartAuxNode = new AuxExpressionParseNode();
    nodesToDelete.push_back(pstartAuxNode);
    AuxExpressionParseNode* pAuxNode = pstartAuxNode;
    pAuxNode->parentNode = nullptr;

    if (token == tokensEnd) {
        std::string aux = *token;
        if ((aux.size() > 2) && (aux.back() == '\"') && (aux.front() == '\"')) {
            auto node = lineOps.CreateConstString();
            node->value.valueString = aux;
            expression->Nodes.push_back(node);
        }
        else if (isInteger(aux)) {
            auto node = lineOps.CreateConstInt();
            node->value.valueInt = std::stoi(aux);
            expression->Nodes.push_back(node);
        }
        else if (isFloat(aux)) {
            auto node = lineOps.CreateConstFloat();
            node->value.valueDouble = std::stod(aux);
            expression->Nodes.push_back(node);
        }
        else if (isBoolean(aux)) {
            auto node = lineOps.CreateConstBool();
            std::string lowerStr = aux;
            std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
            if (lowerStr == "true") {
                node->value.valueBoolean = true;
            }
            else if (lowerStr == "false") {
                node->value.valueBoolean = false;
            }
            expression->Nodes.push_back(node);
        }
        else {
            auto node = lineOps.CreateVar();
            node->name = aux;
            expression->Nodes.push_back(node);
        }
        for (auto& node : nodesToDelete) {
          delete(node);
        }
        return;
    }
    else {
        while (token != tokensEnd) {
            pAuxNode->nodeData.push_back(*token);
            token++;
        }
    }

    std::vector<std::string>::iterator tokenScan;
    std::vector<std::string>::iterator tokenScanEnd;
    std::queue<AuxExpressionParseNode*> NodesToVisit;
    NodesToVisit.push(pAuxNode);


    while (!NodesToVisit.empty()) {
        std::vector<AuxExpressionParseNode*> colapseParentNodes;
        pAuxNode = NodesToVisit.front();
        tokenScan = pAuxNode->nodeData.begin();
        tokenScanEnd = pAuxNode->nodeData.end();
        NodesToVisit.pop();
        while (true) {
            switch (sts) {
            case 0:
                if (*tokenScan == "(") {
                    strVect.clear();
                    countParentesis++;
                    sts = 1;
                }
                break;
            case 1:
                if (*tokenScan == "(") {
                    countParentesis++;
                    strVect.push_back(*tokenScan);
                }
                else if (*tokenScan == ")") {
                    countParentesis--;
                    if (countParentesis == 0) {
                        auto newExpr = new AuxExpressionParseNode();
                        newExpr->parentNode = pAuxNode;
                        nodesToDelete.push_back(newExpr);
                        pAuxNode->childNodes.push_back(newExpr);
                        pAuxNode->childNodes.back()->nodeData = strVect;
                        colapseParentNodes.push_back(newExpr);
                        sts = 0;
                        NodesToVisit.push(newExpr);
                    }
                    else {
                        strVect.push_back(*tokenScan);
                    }
                }
                else {
                    strVect.push_back(*tokenScan);
                }
                break;
            }
            tokenScan++;
            if (tokenScan == tokenScanEnd)
                break;
        }

        for (auto& node : colapseParentNodes) {
            if (node->parentNode != nullptr) {
                colapseParentNodeData(node->parentNode->nodeData, node->nodeData, node->nodeColapsedName);
            }
        }
    }

    std::vector<std::string> operators = { "AND","OR","=",">","<","<>","*","/","+","-" };
    for (auto& op : operators) {
        auto layers = BFSAuxExpressionParse(pstartAuxNode);
        std::vector<AuxExpressionParseNode*> latestLayer;
        while (!layers.empty()) {
            auto layer = layers.top();
            for (auto& mainNode : layer) {
                std::vector<std::string> segment;
                std::vector<std::vector<std::string>> segments;
                for (auto& token : mainNode->nodeData) {
                    if (token == op) {
                        segments.push_back(segment);
                        segment.clear();
                    }
                    else {
                        segment.push_back(token);
                    }
                }
                if (!segment.empty()) {
                    segments.push_back(segment);
                }
                if (segments.size() > 1) {
                    mainNode->nodeFunction = op;
                    mainNode->childNodes.clear();
                    mainNode->nodeData.clear();
                    for (auto& segment : segments) {
                        bool childNodeFound = false;
                        
                        if (!latestLayer.empty()) {
                            for (auto& segmentToken : segment) {
                                for (auto& nodeLastLayer : latestLayer) {
                                    if (nodeLastLayer->nodeColapsedName == segmentToken) {
                                        mainNode->childNodes.push_back(nodeLastLayer);
                                        childNodeFound = true;
                                        break;
                                    }
                                }
                                if (childNodeFound) {
                                    break;
                                }
                            }
                        }
                        if (childNodeFound == false) {
                            auto newchildExpr = new AuxExpressionParseNode();
                            nodesToDelete.push_back(newchildExpr);
                            mainNode->childNodes.push_back(newchildExpr);
                            newchildExpr->nodeData = segment;
                            newchildExpr->parentNode = mainNode;
                            newchildExpr->nodeFunction = "ERROR";
                            if (segment.size() == 1) {
                                if (newchildExpr->nodeData.size() == 1) {
                                    if ((newchildExpr->nodeData[0].size() > 2) && (newchildExpr->nodeData[0].back() == '\"') && (newchildExpr->nodeData[0].front() == '\"')) {
                                        newchildExpr->nodeFunction = "CONST_STRING";
                                    }
                                    else if (isInteger(newchildExpr->nodeData[0])) {
                                        newchildExpr->nodeFunction = "CONST_INT";
                                    }
                                    else if (isFloat(newchildExpr->nodeData[0])) {
                                        newchildExpr->nodeFunction = "CONST_FLOAT";
                                    }
                                    else if (isBoolean(newchildExpr->nodeData[0])) {
                                        newchildExpr->nodeFunction = "CONST_BOOL";
                                    }
                                    else {
                                        newchildExpr->nodeFunction = "VAR";
                                    }
                                }
                            }
                        }
                    }
                }
            }
            latestLayer = layer;
            layers.pop();
        }
    }

    // Uses the auxiliary node tree to build the main node tree
    pAuxNode = pstartAuxNode;
    NodesToVisit.push(pAuxNode);
    while (!NodesToVisit.empty()) {
        pAuxNode = NodesToVisit.front();

        // Create a real node based on data of current auxiliar node                
        if (pAuxNode->nodeFunction == "AND") {

            pAuxNode->realNode = lineOps.CreateAND();
        }
        else if (pAuxNode->nodeFunction == "OR") {
            pAuxNode->realNode = lineOps.CreateOR();
        }
        else if (pAuxNode->nodeFunction == "=" && isCondition) {
            pAuxNode->realNode = lineOps.CreateEqual();
        }
        else if (pAuxNode->nodeFunction == "=" && !isCondition) {
            pAuxNode->realNode = lineOps.CreateAssign();
        }
        else if (pAuxNode->nodeFunction == "<>") {
            pAuxNode->realNode = lineOps.CreateInEqual();
        }
        else if (pAuxNode->nodeFunction == ">") {
            pAuxNode->realNode = lineOps.CreateGreaterThan();
            pAuxNode->realNode->name = ">";
        }
        else if (pAuxNode->nodeFunction == "<") {
            pAuxNode->realNode = lineOps.CreateLessThan();
            pAuxNode->realNode->name = "<";
        }
        else if (pAuxNode->nodeFunction == "+") {
            pAuxNode->realNode = lineOps.CreateADD();
            pAuxNode->realNode->name = "+";
        }
        else if (pAuxNode->nodeFunction == "-") {
            pAuxNode->realNode = lineOps.CreateSUB();
            pAuxNode->realNode->name = "-";
        }
        else if (pAuxNode->nodeFunction == "*") {
            pAuxNode->realNode = lineOps.CreateMUL();
            pAuxNode->realNode->name = "*";
        }
        else if (pAuxNode->nodeFunction == "/") {
            pAuxNode->realNode = lineOps.CreateDIV();
            pAuxNode->realNode->name = "/";
        }
        else if (pAuxNode->nodeFunction == "CONST_STRING") {
            pAuxNode->realNode = lineOps.CreateConstString();
            pAuxNode->realNode->value.valueString = pAuxNode->nodeData[0];
        }
        else if (pAuxNode->nodeFunction == "CONST_INT") {
            pAuxNode->realNode = lineOps.CreateConstInt();
            pAuxNode->realNode->value.valueInt = std::stoi(pAuxNode->nodeData[0]);
        }
        else if (pAuxNode->nodeFunction == "CONST_FLOAT") {
            pAuxNode->realNode = lineOps.CreateConstFloat();
            pAuxNode->realNode->value.valueDouble = std::stod(pAuxNode->nodeData[0]);
        }
        else if (pAuxNode->nodeFunction == "CONST_BOOL") {
            pAuxNode->realNode = lineOps.CreateConstBool();
            std::string lowerStr = pAuxNode->nodeData[0];
            std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
            if (lowerStr == "true") {
                pAuxNode->realNode->value.valueBoolean = true;
            }
            else if (lowerStr == "false") {
                pAuxNode->realNode->value.valueBoolean = false;
            }
        }
        else if (pAuxNode->nodeFunction == "VAR") {
            pAuxNode->realNode = lineOps.CreateVar();
            pAuxNode->realNode->name = pAuxNode->nodeData[0];
        }

        // Check the parent node and stablish the connection with the current node 
        if (pAuxNode->parentNode != nullptr) {
            pAuxNode->parentNode->realNode->Nodes.push_back(pAuxNode->realNode);
        }

        // Add the child nodes for processing later
        for (auto& Node : pAuxNode->childNodes) {
            NodesToVisit.push(Node);
        }
        NodesToVisit.pop();
    }

    expression->Nodes.push_back(pstartAuxNode->realNode);

    // Delete the nodes created
    for (auto& node : nodesToDelete) {
        delete(node);
    }



}

void ParseIF(IFNode* ifn, std::vector<std::string>::iterator token, std::vector<std::string>::iterator tokensEnd, LineOps& lineOps) {
    enum ifparse {
        sts_condition,
        sts_then,
        std_else
    };
    ifparse stsifparse = sts_condition;

    std::vector<std::string>::iterator fromtoken = token;

    Node* thenn = nullptr;
    Node* elsen = nullptr;
    ExpressionNode* expressionn = nullptr;

    while (true) {
        switch (stsifparse) {
        case sts_condition:
            if ((*token == "THEN") || (*token == "then")) {
                expressionn = lineOps.CreateExpression();
                ParseEXPRESSION(expressionn, fromtoken, token, lineOps, true);
                stsifparse = sts_then;
                fromtoken = token + 1;
            }
            break;

        case sts_then:
            if ((*token == "ELSE") || (*token == "else")) {
                thenn = ParseLine(fromtoken, token, lineOps);
                stsifparse = std_else;
                fromtoken = token + 1;
            }
            break;
        }

        if (token + 1 == tokensEnd) {
            if (stsifparse == std_else) {
                elsen = ParseLine(fromtoken, tokensEnd, lineOps);
            }
            else if (stsifparse == sts_then) {
                thenn = ParseLine(fromtoken, tokensEnd, lineOps);
            }
            break;
        }
        token++;
    }

    ifn->AddNodes(expressionn, thenn, elsen);
}

// FOR I = 1 TO 10 : PRINT I : NEXT
// FOR I = 10 TO 1 STEP -1 : PRINT I : NEXT
void ParseFOR(FORNode* fn, std::vector<std::string>::iterator token, std::vector<std::string>::iterator tokensEnd, LineOps& lineOps) {
    enum forparse {
        sts_init_var,
        sts_var_value,
        sts_var_limit,
        sts_action_next_step
    };
    forparse stsforparse = sts_init_var;

    std::vector<std::string>::iterator fromtoken = token;

    Node* varInit = lineOps.CreateNode();
    Node* varFinish = lineOps.CreateNode();
    Node* action = nullptr;
    Node* nextStep = lineOps.CreateNode();
    std::string lastToken;

    while (token != tokensEnd) {
        switch (stsforparse) {
        case sts_init_var:
            varInit->name = *token;
            stsforparse = sts_var_value;
            break;

        case sts_var_value:
            if (*token != "=") {
                varInit->value.type = Var::varType::v_integer;
                varInit->value.valueInt = std::stoi(*token);
                stsforparse = sts_var_limit;
            }
            break;

        case sts_var_limit:
            if ((*token != "TO") && (*token != "to")) {
                varFinish->name = "limit";
                varFinish->value.type = Var::varType::v_integer;
                varFinish->value.valueInt = std::stoi(*token);
                fromtoken = token + 1;
                stsforparse = sts_action_next_step;
            }
            break;

        case sts_action_next_step:
            if ((*token == "NEXT") || (*token == "next") || (*token == "STEP") || (*token == "step")) {
                action = ParseLine(fromtoken, token, lineOps);
                lastToken = *token;
            }
            else {
                nextStep->name = "next_step";
                nextStep->value.type = Var::varType::v_integer;
                if ((lastToken == "STEP") || (lastToken == "step")) {
                    if (*token == "-") {
                        token++;
                        nextStep->value.valueInt = -std::stoi(*token);
                    }
                    else {
                        nextStep->value.valueInt = std::stoi(*token);
                    }
                }
                else {
                    nextStep->value.valueInt = 1;
                }
            }
            break;
        }
        token++;
    }

    fn->AddNodes(varInit, varFinish, action, nextStep);
}

//WHILE y < 5 : x = x+1
void ParseWHILE(WHILENode* wn, std::vector<std::string>::iterator token, std::vector<std::string>::iterator tokensEnd, LineOps& lineOps) {
    enum whileparse {
        sts_condition,
        sts_action
    };
    whileparse stswhileparse = sts_condition;

    std::vector<std::string>::iterator fromtoken = token;

    ExpressionNode* expressionn = nullptr;
    Node* action = nullptr;

    while (token != tokensEnd) {
        switch (stswhileparse) {
        case sts_condition:
            if (*token == ":") {
                expressionn = lineOps.CreateExpression();
                ParseEXPRESSION(expressionn, fromtoken, token, lineOps, true);
                fromtoken = token + 1;
                stswhileparse = sts_action;
            }
            break;
        case sts_action:
            if (*token == "WEND" || *token == "wend") {
                action = ParseLine(fromtoken, token, lineOps);
            }
            break;
        }
        token++;
    }

    wn->AddNodes(expressionn, action);
}

void ParseGoto(GotoNode* gtn, std::vector<std::string>::iterator token, std::vector<std::string>::iterator tokensEnd, LineOps& lineOps) {
    gtn->value.valueInt = std::stoi(*token);
}

void ParseGosub(GosubNode* gsn, std::vector<std::string>::iterator token, std::vector<std::string>::iterator tokensEnd, LineOps& lineOps) {
    gsn->value.valueInt = std::stoi(*token);
}

void ParsePrint(FncPrintNode* pn, std::vector<std::string>::iterator token, std::vector<std::string>::iterator tokensEnd, LineOps& lineOps) {

    Node* lastNode = pn;
    ExpressionNode* expressionn = nullptr;
    std::vector<std::string>::iterator begintoken = token;
    std::vector<std::string>::iterator fromtoken = tokensEnd;
    bool stringFound = false;
    bool semicolonFound = false;

    while (token != tokensEnd) {
        std::string aux = *token;
        if (aux.size() > 2 && aux.front() == '\"' && aux.back() == '\"') {
            stringFound = true;
            auto newnode = lineOps.CreatePrint();
            newnode->value.type = Var::varType::v_string;
            newnode->value.valueString = aux.substr(1, aux.length() - 2);
            lastNode->linkedNode = newnode;
            lastNode = newnode;
        }
        else {
            if (aux == ";") {
                semicolonFound = true;
                if (fromtoken == tokensEnd) {
                    fromtoken = token;
                }
                else {
                    auto newnode = lineOps.CreateExpression();
                    ParseEXPRESSION(newnode, fromtoken + 1, token - 1, lineOps, true);
                    lastNode->linkedNode = newnode;
                    lastNode = newnode;
                    fromtoken = tokensEnd;
                }
            }
        }
        token++;
    }

    if (fromtoken != tokensEnd) {
        auto newnode = lineOps.CreateExpression();
        if (tokensEnd - fromtoken > 2) {
            ParseEXPRESSION(newnode, fromtoken + 1, tokensEnd, lineOps, true);
        }
        else {
            ParseEXPRESSION(newnode, fromtoken + 1, fromtoken + 1, lineOps, true);
        }
        lastNode->linkedNode = newnode;        
    }
    // expression/var only 
    else if (stringFound == false && semicolonFound == false) {        
        auto newnode = lineOps.CreateExpression();
        if (tokensEnd - begintoken > 2) {
            ParseEXPRESSION(newnode, begintoken, tokensEnd, lineOps, true);
        }
        else {
            ParseEXPRESSION(newnode, begintoken , begintoken, lineOps, true);
        }        
        lastNode->linkedNode = newnode;        
    }
}

void ParseColor(ColorNode* pn, std::vector<std::string>::iterator token, std::vector<std::string>::iterator tokensEnd, LineOps& lineOps) {
    std::vector<std::string>::iterator fromtoken = token;
    bool semicolonfond = false;
    Node* lastNode = pn;
    while (token != tokensEnd) {
        if (!semicolonfond && *token == ",") {
            semicolonfond = true;
            auto newnode = lineOps.CreateExpression();
            ParseEXPRESSION(newnode, fromtoken, token - 1, lineOps, true);
            fromtoken = token + 1;
            lastNode->linkedNode = newnode;
            lastNode = newnode;
        }
        token++;
    }
    if (token != fromtoken) {
        auto newnode = lineOps.CreateExpression();
        ParseEXPRESSION(newnode, fromtoken, tokensEnd - 1, lineOps, true);
        lastNode->linkedNode = newnode;
    }
}

void ParseLocate(LocateNode* pn, std::vector<std::string>::iterator token, std::vector<std::string>::iterator tokensEnd, LineOps& lineOps) {
    std::vector<std::string>::iterator fromtoken = token;
    bool semicolonfond = false;
    Node* lastNode = pn;
    while (token != tokensEnd) {
        if (!semicolonfond && *token == ",") {
            semicolonfond = true;
            auto newnode = lineOps.CreateExpression();
            ParseEXPRESSION(newnode, fromtoken, token - 1, lineOps, true);
            fromtoken = token + 1;
            lastNode->linkedNode = newnode;
            lastNode = newnode;
        }
        token++;
    }
    if (token != fromtoken) {
        auto newnode = lineOps.CreateExpression();
        ParseEXPRESSION(newnode, fromtoken, tokensEnd - 1, lineOps, true);
        lastNode->linkedNode = newnode;
    }
}

void ParseCircle(CircleNode* pn, std::vector<std::string>::iterator token, std::vector<std::string>::iterator tokensEnd, LineOps& lineOps) {
    std::vector<std::string>::iterator fromtoken = token;
    Node* lastNode = pn;
    while (token != tokensEnd) {
        if (*token == ",") {
            auto newnode = lineOps.CreateExpression();
            ParseEXPRESSION(newnode, fromtoken, token - 1, lineOps, true);
            fromtoken = token + 1;
            lastNode->linkedNode = newnode;
            lastNode = newnode;
        }
        token++;
    }
    if (token != fromtoken) {
        auto newnode = lineOps.CreateExpression();
        ParseEXPRESSION(newnode, fromtoken, tokensEnd - 1, lineOps, true);
        lastNode->linkedNode = newnode;
    }
}

void ParseLine(LineNode* pn, std::vector<std::string>::iterator token, std::vector<std::string>::iterator tokensEnd, LineOps& lineOps) {
    std::vector<std::string>::iterator fromtoken = token;
    Node* lastNode = pn;
    while (token != tokensEnd) {
        if (*token == ",") {
            auto newnode = lineOps.CreateExpression();
            ParseEXPRESSION(newnode, fromtoken, token - 1, lineOps, true);
            fromtoken = token + 1;
            lastNode->linkedNode = newnode;
            lastNode = newnode;
        }
        token++;
    }
    if (token != fromtoken) {
        auto newnode = lineOps.CreateExpression();
        ParseEXPRESSION(newnode, fromtoken, tokensEnd - 1, lineOps, true);
        lastNode->linkedNode = newnode;
    }
}

void ParseSleep(SleepNode* pn, std::vector<std::string>::iterator token, std::vector<std::string>::iterator tokensEnd, LineOps& lineOps) {
    auto newnode = lineOps.CreateExpression();
    ParseEXPRESSION(newnode, token, tokensEnd - 1, lineOps, true);
    pn->linkedNode = newnode;
}

Node* ParseLine(std::vector<std::string>::iterator token, std::vector<std::string>::iterator tokensEnd, LineOps& lineOps) {

    std::string tk = *token;
    std::transform(tk.begin(), tk.end(), tk.begin(),[](unsigned char c) { return std::tolower(c); });

    if (tk == "if") {
        IFNode* ifn = lineOps.CreateIF();
        if (++token >= tokensEnd) return(nullptr);
        ParseIF(ifn, token, tokensEnd, lineOps);
        return(ifn);
    }
    if (tk == "for") {
        FORNode* fn = lineOps.CreateFOR();
        if (++token >= tokensEnd) return(nullptr);
        ParseFOR(fn, token, tokensEnd, lineOps);
        return(fn);
    }
    if (tk == "while") {
        WHILENode* wn = lineOps.CreateWHILE();
        if (++token >= tokensEnd) return(nullptr);
        ParseWHILE(wn, token, tokensEnd, lineOps);
        return(wn);
    }
    if (tk == "goto") {
        GotoNode* gtn = lineOps.CreateGotoNode();
        if (++token >= tokensEnd) return(nullptr);
        ParseGoto(gtn, token, tokensEnd, lineOps);
        return(gtn);
    }    
    if (tk == "gosub") {
        GosubNode* gsn = lineOps.CreateGosubNode();
        if (++token >= tokensEnd) return(nullptr);
        ParseGosub(gsn, token, tokensEnd, lineOps);
        return(gsn);
    }
    if (tk == "return") {
        ReturnNode* rtn = lineOps.CreateReturnNode();
        return(rtn);
    }
    if (tk == "print") {
        FncPrintNode* pn = lineOps.CreatePrint();
        if (++token >= tokensEnd) return(nullptr);
        ParsePrint(pn, token, tokensEnd, lineOps);
        return(pn);
    }
    if (tk == "input") {
        //IFNode* ifn = lineOps.CreateIF();
        //ParseIF(ifn, token, tokensEnd, lineOps, globalVars);
        //Node = ifn;
    }
    if (tk == "color") {
        ColorNode* n = lineOps.CreateColor();
        if (++token >= tokensEnd) return(nullptr);
        ParseColor(n, token, tokensEnd, lineOps);
        return(n);
    }

    if (tk == "locate") {
        LocateNode* n = lineOps.CreateLocate();
        if (++token >= tokensEnd) return(nullptr);
        ParseLocate(n, token, tokensEnd, lineOps);
        return(n);
    }

    if (tk == "circle") {
        CircleNode* n = lineOps.CreateCircle();
        if (++token >= tokensEnd) return(nullptr);
        ParseCircle(n, token, tokensEnd, lineOps);
        return(n);
    }

    if (tk == "line") {
        LineNode* n = lineOps.CreateLine();
        if (++token >= tokensEnd) return(nullptr);
        ParseLine(n, token, tokensEnd, lineOps);
        return(n);
    }

    if (tk == "sleep") {
        SleepNode* n = lineOps.CreateSleep();
        if (++token >= tokensEnd) return(nullptr);
        ParseSleep(n, token, tokensEnd, lineOps);
        return(n);
    }

    if (tk == "end") {
        EndNode* n = lineOps.CreateEnd();
        return(n);
    }

    if (tk == "cls") {
        ClsNode* n = lineOps.CreateCls();
        return(n);
    }

    ExpressionNode* expression = lineOps.CreateExpression();
    ParseEXPRESSION(expression, token, tokensEnd, lineOps, false);
    return(expression);

}

void Debug(int lineNumber, Global& global) {
    std::cout << std::endl << "Line: " << lineNumber << std::endl;
    std::cout << "Vars:" << std::endl;
    for (auto& var : global.Vars) {
        switch (var.second.type) {
        case Var::varType::v_string: {
            std::cout << "name '" << var.first << "'" << "type: v_string value:" << var.second.valueString << std::endl;
            break;
        }
        case Var::varType::v_boolean: {
            std::cout << "name '" << var.first << "'" << "type: v_boolean value:" << var.second.valueBoolean << std::endl;
            break;
        }
        case Var::varType::v_double: {
            std::cout << "name '" << var.first << "'" << "type: v_double value:" << var.second.valueDouble << std::endl;
            break;
        }
        case Var::varType::v_integer: {
            std::cout << "name '" << var.first << "'" << "type: v_integer value:" << var.second.valueInt << std::endl;;
            break;
        }
        }
    }
}

bool LoadProgram() {
    bool lineNumberFound = false;
    std::string LineStr;
    int lineNumber;
    for (auto& c : ProgStr) {
        if (!lineNumberFound) {
            if (charIsNum(c))
                LineStr += c;
            else {
                try {
                    lineNumber = std::stoi(LineStr);
                    LineStr = "";
                    lineNumberFound = true;
                }
                catch (...) {
                    return false;
                }
            }
        }
        else {
            if (c != '\n') {
                LineStr += c;
            }
            else {
                program[lineNumber] = Tokenize(LineStr);
                LineStr = "";
                lineNumberFound = false;
            }
        }
    }
    return true;
}



uint16_t basicColorToTFT(int basicColor) {
#if !PCMODE
    switch (basicColor) {
    case 0: return TFT_BLACK;
    case 1: return TFT_BLUE;
    case 2: return TFT_GREEN;
    case 3: return TFT_CYAN;
    case 4: return TFT_RED;
    case 5: return TFT_MAGENTA;
    case 6: return TFT_MAROON;
    case 7: return TFT_WHITE;
    case 8: return TFT_LIGHTGREY;
    case 9: return TFT_ORANGE;
    default: return TFT_BLACK; 
    }
#else
    return 0;
#endif

}


void FncPrint(Global& global) {
#if VERBOSE
    std::cout << "FncPrint:" << global.print << std::endl;
#endif
#if !PCMODE
    tft.println(global.print.c_str());    
#endif
    global.print.clear();
}

void FncSleep(Global& global) {
#if VERBOSE
    std::cout << "FncSleep:" << global.par1 << std::endl;
#endif
#if !PCMODE
    delay(global.par1);
#endif
}

void FncLine(Global& global) {
#if VERBOSE
    std::cout << "FncLine: (" << global.x1 << "," << global.y1 << "," << global.x2 << "," << global.y2 << ") " << global.color1 << std::endl;
#endif
#if !PCMODE
    tft.drawLine(global.x1, global.y1, global.x2, global.y2, basicColorToTFT(global.color1));
#endif
}

void FncCircle(Global& global) {
#if VERBOSE
    std::cout << "FncCircle: (" << global.x1 << "," << global.y1 << ") " << global.x2 << "," << basicColorToTFT(global.color1) << std::endl;
#endif
#if !PCMODE
    tft.drawCircle(global.x1, global.y1, global.x2, basicColorToTFT(global.color1));
#endif
}

void FncLocate(Global& global) {
#if VERBOSE
    std::cout << "FncLocate:" << std::endl;
#endif
#if !PCMODE
    tft.setCursor(global.x1, global.y1);
#endif
}

void FncColor(Global& global) {
#if VERBOSE
    std::cout << "FncColor:" << std::endl;
#endif
#if !PCMODE
    tft.setTextColor(basicColorToTFT(global.color1), basicColorToTFT(global.color2));
#endif
}

void FncClearScreen(Global& global) {
#if VERBOSE
    std::cout << "FncClearScreen:" << std::endl;
#endif
#if !PCMODE
    tft.fillScreen(TFT_BLACK);
#endif
}

void PrintInfoMessage(std::string info) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK); 
  tft.setTextSize(1); 
  tft.setCursor(0, 20);
  tft.println(info.c_str());  
  tft.setCursor(0, 0);
}

void PrintResetCursor() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(0, 20);
  tft.setTextSize(1);
}

void PrintLn(std::string msg) {
  tft.println(msg.c_str());  
}

void PrintErrorMessage(std::string error) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED, TFT_BLACK); 
  tft.setTextSize(1); 
  tft.setCursor(0, 20);
  tft.println(error.c_str());  
  tft.setCursor(0, 0);
}


void RunProgram() {

    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK); 
    tft.setTextSize(1); 
    tft.setCursor(0, 0);

    auto it = program.begin();
    int syntaxErrorLine = -1;
    int runtimeErrorLine = -1;
    Global globalVars;
    Node* mainNode = nullptr;

    // Parse the lines and create the AST
    while (it != program.end()) {

        int line = it->first;
        auto& tokens = it->second;
        LineOps lineOps;
        Node* mainNode = nullptr;
        std::vector<std::string>::iterator itTokens = it->second.begin();
        std::vector<std::string>::iterator tokensEnd = it->second.end();

        globalVars.currentLine = line;
        globalVars.repeatCurrentLine = false;
        globalVars.enableGotoReturnLine = false;
        globalVars.enableCallLine = false;
        globalVars.finishProgram = false;
        globalVars.error.clear();

        try {
            mainNode = ParseLine(itTokens, tokensEnd, lineOps);
        }
        catch (...) {
            syntaxErrorLine = line;
            break;
        }
       
        // Run line
        if (mainNode != nullptr) {
            try {
                mainNode->eval(globalVars);
            }
            catch (...) {
                runtimeErrorLine = line;
            }

            if (mainNode->typeNode == "FncPrintNode") {
                FncPrint(globalVars);
            }
            else if (mainNode->typeNode == "LocateNode") {
                FncLocate(globalVars);
            }
            else if (mainNode->typeNode == "ColorNode") {
                FncColor(globalVars);
            }
            else if (mainNode->typeNode == "LineNode") {
                FncLine(globalVars);
            }
            else if (mainNode->typeNode == "CircleNode") {
                FncCircle(globalVars);
            }
            else if (mainNode->typeNode == "SleepNode") {
                FncSleep(globalVars);
            }
            else if (mainNode->typeNode == "ClsNode") {
                FncClearScreen(globalVars);
            }
        }

        // Stop on error
        if (!globalVars.error.empty()) {
            std::cout << "Runtime error in line " << globalVars.currentLine << " : " << globalVars.error << std::endl;
            break;
        }

        if (globalVars.enableGotoReturnLine || globalVars.enableCallLine) {
            auto repeat = it;
            auto next = ++it;
            it = program.find(globalVars.callGotoReturnLine);
            if (it == program.end()) {
                std::cout << "Runtime error, line: " << globalVars.callGotoReturnLine << " not found" << std::endl;
                break;
            }
            if (globalVars.enableCallLine) {
                if (globalVars.repeatCurrentLine) {
                    globalVars.lineStack.push(repeat->first);
                }
                else if (next != program.end()) {
                    globalVars.lineStack.push(next->first);
                }
            }
        }
        else if (!globalVars.repeatCurrentLine) {
            // Load the next line
            it++;
        }

        if (globalVars.finishProgram) {
            it = program.end();
        }    
    }
}


void SaveFile(const std::string& fileName) {
  std::string programSave;
  for(const auto& line : program) {  
    int lineNumber = line.first;
    programSave += std::to_string(lineNumber) + " ";
    auto tokens = line.second;
    for (const auto& token : tokens) {
      programSave += token + " ";
    }
    programSave += "\n";
  }
  std::string fileName_ = "/" + fileName;
  const char* filePath = fileName_.c_str();
  File file = SPIFFS.open(filePath, FILE_WRITE);
  if (!file) {
    PrintErrorMessage("Error opening file " + fileName); 
    return;
  }
  if (file.print(programSave.c_str())) {
    PrintInfoMessage(fileName + " file written successfully");      
  } else {
    PrintErrorMessage("Error writing file " + fileName);    
  }
  file.close();
}

void LoadFile(const std::string& fileName){ 
  std::string fileName_ = "/" + fileName;
  const char* filePath = fileName_.c_str();  
  File file = SPIFFS.open(filePath, FILE_READ);
  if (!file) {
    PrintErrorMessage("Error opening file " + fileName);    
    return ; 
  }  
  ProgStr.clear();
  while (file.available()) {
    ProgStr += file.readStringUntil('\n').c_str(); 
    ProgStr += "\n"; 
  }
  file.close();
  program.clear();
  if(LoadProgram())
    PrintInfoMessage(fileName + " file loaded successfully");
  else
    PrintErrorMessage("Error processing file " + fileName); 
}


void Dir() {
  File root = SPIFFS.open("/");
  if (!root) {
    PrintErrorMessage("Error opening the file system");    
    return;
  }
  PrintResetCursor();
  File file = root.openNextFile();
  while (file) {
    PrintLn(file.name());
    file = root.openNextFile();
  }
}

void Delete(const std::string& fileName) {

  std::string fileName_ = "/" + fileName;
  const char* filePath = fileName_.c_str(); 
  
  if (SPIFFS.exists(filePath)) {    
    if (SPIFFS.remove(filePath)) {
       PrintInfoMessage(fileName + " file deleted successfully");
    } else {
      PrintErrorMessage("Failed to delete " + fileName_ + " file.");      
    }
  } else {
    PrintErrorMessage("File " + fileName_ + " not found."); 
  }
}

void List(int Begin, int End) {
  std::cout << "Begin: " << Begin << " End: " << End << std::endl;

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK); 
  tft.setTextSize(1); 
  tft.setCursor(0, 20);

  int maxLines = 20;
  if(End == -1) { // Begin + limit from it
      for (auto& line : program) {
        if(line.first >= Begin && maxLines) {          
          std::string str = std::to_string(line.first) + " ";
          for (auto token : line.second) {
            str += token + " ";
          }
          tft.println(str.c_str());          
          maxLines--;
        }
      }
  } else if (End == -2) { // Single line
      if(program.find(Begin) != program.end()) {
        auto lineTokens = program[Begin];
        std::string str = std::to_string(Begin) + " ";
        for (auto token : lineTokens) {
          str += token + " ";
        }
        tft.println(str.c_str());    
      }
  } else { // Begin to end
    for (auto& line : program) {
      if(line.first >= Begin && maxLines && line.first <= End) {          
        std::string str = std::to_string(line.first) + " ";
        for (auto token : line.second) {
          str += token + " ";
        }
        tft.println(str.c_str());          
        maxLines--;
      }
    }
  }
}

void setup(void) {
  tft.init();

  tft.setRotation(1);
  Serial.begin(9600);
  Serial.println("ready");
  tft.fillScreen(TFT_BLACK);

  if (!SPIFFS.begin(true)) 
  { 
    PrintErrorMessage("File system initialization failed.");      
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}


void loop() {

  if(stringComplete) {
    stringComplete = false;
    if(!inputString.empty()) {
      std::vector<std::string> tokens = Tokenize(inputString);
      inputString.clear();
      std::string token = tokens[0];
      std::transform(token.begin(), token.end(), token.begin(),[](unsigned char c) { return std::tolower(c); });
      std::string arg; 
      for (size_t i = 1; i < tokens.size(); ++i) {
        arg += tokens[i];
      }
      if(token == "run") {       
        RunProgram(); 
      } else if (token == "load") {
        LoadFile(arg);        
      } else if (token == "save") {
        SaveFile(arg);        
      } else if (token == "list") {
        int Begin = std::stoi(tokens[1]);        
        if(tokens.size() == 4){     // LIST 10-20               
          int End = std::stoi(tokens[3]);
          List(Begin,End); 
        }else if(tokens.size() == 3){ // LIST 100-          
          List(Begin, -1);
        } else if(tokens.size() == 2){ // LIST 50          
          List(Begin, -2);
        } else {
          PrintErrorMessage("Error: Missing arguments.");
        }

      } else if (token == "dir") {
        Dir();
      }
      else if (token == "delete") {
        Delete(arg);
      }
      else if (token == "new") { 
        program.clear();
      }
      else if(isInteger(token)) {
        int lineNumber = std::stoi(token);
        std::cout << "p0:" << lineNumber << std::endl;
        // Delete line
        if(tokens.size() == 1) {
          std::cout << "p1: delete" << std::endl;
          program.erase(lineNumber);
        } else {
          std::cout << "p2: insert arg:" << arg << std::endl;
          tokens.erase(tokens.begin());
          program[lineNumber] = tokens;
        }
      }
    }
  }
}



