#include "Parser.h"
#include "Solver.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

double Parser::parseValue(const std::string& token) {
    std::size_t used=0; double value=0.;
    try { value=std::stod(token,&used); } catch(...) { throw std::invalid_argument("Invalid number '"+token+"'"); }
    std::string suffix=token.substr(used); std::transform(suffix.begin(),suffix.end(),suffix.begin(),[](unsigned char c){return std::tolower(c);});
    static const std::unordered_map<std::string,double> factors{{"",1},{"t",1e12},{"g",1e9},{"meg",1e6},{"k",1e3},{"m",1e-3},{"u",1e-6},{"n",1e-9},{"p",1e-12},{"f",1e-15}};
    auto it=factors.find(suffix); if(it==factors.end())throw std::invalid_argument("Unknown engineering suffix '"+suffix+"'"); return value*it->second;
}
void Parser::parseFile(const std::string& filePath,Solver&solver)const{
    std::ifstream in(filePath);if(!in)throw std::runtime_error("Unable to open netlist: "+filePath);std::string line;int number=0;
    while(std::getline(in,line)){++number;auto semicolon=line.find(';');if(semicolon!=std::string::npos)line.resize(semicolon);std::istringstream input(line);std::vector<std::string>t;for(std::string s;input>>s;)t.push_back(s);if(t.empty()||t[0][0]=='*'||t[0][0]=='#')continue;
        try {std::string head=t[0];std::transform(head.begin(),head.end(),head.begin(),[](unsigned char c){return std::tolower(c);});if(head==".end")break;if(head==".tran"){if(t.size()!=3)throw std::runtime_error(".tran requires step and stop");solver.setTransient(parseValue(t[1]),parseValue(t[2]));continue;}if(head==".ac"){if(t.size()!=5)throw std::runtime_error(".ac requires mode, points, start and stop");std::string mode=t[1];std::transform(mode.begin(),mode.end(),mode.begin(),[](unsigned char c){return std::tolower(c);});if(mode!="dec")throw std::runtime_error("only .ac dec sweeps are supported");solver.setAcSweep(std::stoi(t[2]),parseValue(t[3]),parseValue(t[4]));continue;}
            const char k=static_cast<char>(std::toupper(static_cast<unsigned char>(t[0][0])));Solver::Element e{};e.name=t[0];auto node=[&](int i){return std::stoi(t.at(i));};
            if(k=='R'||k=='C'||k=='L'||k=='I'||k=='V'){if(t.size()<4)throw std::runtime_error("requires two nodes and a value");e.p=node(1);e.n=node(2);e.kind=k=='R'?Solver::Kind::Resistor:k=='C'?Solver::Kind::Capacitor:k=='L'?Solver::Kind::Inductor:k=='I'?Solver::Kind::Current:Solver::Kind::Voltage;int i=3;if(i<t.size()){std::string dc=t[i];std::transform(dc.begin(),dc.end(),dc.begin(),[](unsigned char c){return std::tolower(c);});if(dc=="dc")++i;}if(i>=t.size())throw std::runtime_error("missing source value");e.value=parseValue(t[i++]);if(i<t.size()){std::string ac=t[i++];std::transform(ac.begin(),ac.end(),ac.begin(),[](unsigned char c){return std::tolower(c);});if(ac!="ac"||i>=t.size())throw std::runtime_error("expected AC magnitude [phase]");e.acMagnitude=parseValue(t[i++]);if(i<t.size())e.acPhaseDegrees=parseValue(t[i++]);}if(i!=t.size())throw std::runtime_error("too many fields");}
            else if(k=='G'||k=='E'){if(t.size()!=6)throw std::runtime_error("requires output nodes, control nodes, and gain");e.p=node(1);e.n=node(2);e.cp=node(3);e.cn=node(4);e.value=parseValue(t[5]);e.kind=k=='G'?Solver::Kind::Vccs:Solver::Kind::Vcvs;}
            else if(k=='F'||k=='H'){if(t.size()!=5)throw std::runtime_error("requires output nodes, controlling source, and gain");e.p=node(1);e.n=node(2);e.controlName=t[3];e.value=parseValue(t[4]);e.kind=k=='F'?Solver::Kind::Cccs:Solver::Kind::Ccvs;}
            else throw std::runtime_error("Unsupported component '"+t[0]+"'");solver.addElement(std::move(e));
        }catch(const std::exception&e){throw std::runtime_error("Netlist line "+std::to_string(number)+": "+e.what());}}
}
