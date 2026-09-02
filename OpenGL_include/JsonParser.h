
#pragma once
#include <fstream>
#include <iomanip>
#include <limits>
#include <string>
#include <unordered_map>
#include <utility>
#include<variant>
#include <vector>

#include <stdexcept>



class JsonValue
{
public:

  using Null    = std::monostate;
  using Boolean = bool;
  using Number  = double;
  using String  = std::string;
  using Array = std::vector<JsonValue>;
  using Object= std::unordered_map<std::string,JsonValue>;
  using ValueVariant =std::variant<Null,Boolean,Number,String,Array,Object>;

  explicit JsonValue(Null null) :value(null){}
  explicit JsonValue(Number number) :value(number){}
  explicit JsonValue(Boolean boolean) :value(boolean){}
  explicit JsonValue(String string) :value(string){}
  explicit JsonValue(Array array) :value(array){}
  explicit JsonValue(Object object) :value(object){}
  explicit JsonValue(ValueVariant variant) :value(std::move(variant)){}
  explicit JsonValue():value(std::monostate()){}


  JsonValue & operator[](const std::string& key ) {//when acessing value with key
    Object & obj=std::get<Object>(value);
    return obj[key];
  }
  JsonValue & operator[](const int index ) {//when acessing array with index
    Array &array=std::get<Array>(value);
    return array[index];

  }
  struct JsonPrinter {
    std::ostream& os; // We store the stream here so we can print to it!

    void operator()(const Number n) const {
      os << std::setprecision(std::numeric_limits<double>::max_digits10) << n;
    }
    void operator()(const String &str) const {
      os <<'"'<<str<<'"';
    }
    void operator()(Boolean b) const {
      if (b)os<<"true";
      else os<<"false";
    }
    void operator()(Null null) const {
      os <<"null";
    }
    void operator()(const Array& arr) const {
      os<<"[";
      for (int i=0;i<arr.size();i++) {
        os<<arr[i];
        if (i<arr.size()-1) os<<",";
      }
      os<<"]";
    }

    void operator()(const Object& obj) const {
      os<<"{"<<std::endl;
      int i=0;
      for (const auto &ob:obj) {

        os<<'"'<<ob.first<<'"'<<": "<<ob.second;
        if (i++<obj.size()-1) os<<","<<std::endl;
      }
      os<<"}";
    }

  };
  friend std::ostream& operator<<(std::ostream& os,const  JsonValue& json_value) {
    std::visit(JsonPrinter{os},json_value.value);
    return os;
  }
  double as_number() {
    Number number=std::get<Number>(value);
    return  number;
  }
  std::string as_string() {
    String string=std::get<String>(value);
    return string;
  }



private:
  ValueVariant value;
};


enum class TokenType
{
  LeftBrace,
  String,
  Colon,
  RightBrace,
  LeftBracket,
  RightBracket,
  Number,
  Bool,
  Null,
  Comma,
};

struct Token
{
  TokenType type;
  std::string content;

  Token(TokenType t,std::string str):type(t) ,content(std::move(str)){}

};

class JsonTokenizer
{
  public:
  JsonTokenizer(std::string_view string): json_text(string){}
  std::vector<Token> tokens;
  void Tokenize()
  {
    while (position < json_text.size())
    {
      char current_char=json_text[position];
      switch (current_char)
      {
      case '{':
        tokens.emplace_back(TokenType::LeftBrace, "{");
        position++;
        break;

      case '}':
        tokens.emplace_back(TokenType::RightBrace, "}");
        position++;
        break;

      case '[':
        tokens.emplace_back(TokenType::LeftBracket, "[");
        position++;
        break;

      case ']':
        tokens.emplace_back(TokenType::RightBracket, "]");
        position++;
        break;

      case ':':
        tokens.emplace_back(TokenType::Colon, ":");
        position++;
        break;
      case ',':
        tokens.emplace_back(TokenType::Comma, ",");
        position++;
        break;
      case '"':
        std::string str;
        position++;
        while ( position<json_text.size() && json_text[position]!='"')
        {
          str+=json_text[position];
          position++;
        }
        tokens.emplace_back(TokenType::String,str);
        position++;
        break;
      }
      if (current_char=='-' || std::isdigit(current_char))
      {
        std::string str;
        str+=json_text[position];
        position++;
        bool dot_flag=false;
        while ( position<json_text.size() && (std::isdigit(json_text[position])||json_text[position]=='.' && !dot_flag ) )
        {
          if (json_text[position]=='.') dot_flag=true;
          str+=json_text[position];
          position++;
        }
        tokens.emplace_back(TokenType::Number,str);
      }

      if (std::isalpha(current_char))
      {
        switch (current_char)
        {
        case 't':
        case 'n':
          if ( position+3<json_text.size())
          {
            std::string str;
            for (int i =0;i<4;i++) str+=json_text[position+i];
            if (str=="true") tokens.emplace_back(TokenType::Bool,str);
            else if (str=="null") tokens.emplace_back(TokenType::Null,str);
            else throw std::runtime_error("Invalid JSON keyword");

            position+=4;
          }
          break;

        case 'f':
          if ( position+4<json_text.size())
          {
            std::string str;
            for (int i =0;i<5;i++) str+=json_text[position+i];
            if (str=="false") tokens.emplace_back(TokenType::Bool,str);
            else throw std::runtime_error("Invalid JSON keyword");

            position+=5;
          }
          break;
        default:
          throw std::runtime_error("Invalid JSON keyword");
        }
      }
      if (std::isspace(current_char)) position++;
    }
  }

  Token* getTokenAtIndex(unsigned int index) {return &tokens[index];}

  void setJsonText(std::string &file_string) {
    json_text=file_string;
    Tokenize();
  }

private:
  std::string_view json_text;
  unsigned int position=0;
};

class JsonParser
{
public:
  JsonParser() : tokenizer("") {}
  void parseFile(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file) {
      throw std::runtime_error("Could not open file");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file_string=buffer.str();
    tokenizer.setJsonText(file_string);
  }
  JsonValue parse()
  {

    current_index=0;
    JsonValue root=parseValue();
    this->root=root;
    return root;

  }


private:
  std::string file_string;
  JsonValue root;
  JsonTokenizer tokenizer;
  size_t current_index=0;
  Token *getCurrentToken(){return &tokenizer.tokens[current_index]; }

  JsonValue parseObject()
  {
  std::unordered_map<std::string,JsonValue> object;
  if (tokenizer.tokens[current_index+1].type==TokenType::RightBrace)
  {
    current_index+=2;
    return JsonValue(object);
  }
  do
  {
    current_index++;
    if (getCurrentToken()->type==TokenType::String)
    {

      std::string key=  getCurrentToken()->content;
      current_index++;
      if(getCurrentToken()->type==TokenType::Colon)
      {
        current_index++;
        object[key]=parseValue();
        if (getCurrentToken()->type==TokenType::RightBrace)
        {
          current_index++;
          return JsonValue(object);
        }

      }
    }
  }while (getCurrentToken()->type==TokenType::Comma);
  current_index++;
  return JsonValue(object);
  }

  JsonValue parseArray()
  {
    std::vector<JsonValue> arr;
    if (tokenizer.tokens[current_index+1].type==TokenType::RightBracket)
    {
      current_index+=2;
      return JsonValue(arr);

    }
    do
    {
      current_index++;
      arr.emplace_back(parseValue());
      if (getCurrentToken()->type==TokenType::RightBracket)
      {
        current_index++;
        return JsonValue(arr);
      }
    }while (getCurrentToken()->type==TokenType::Comma);

  return JsonValue(arr);
  }


  JsonValue parseValue()
  {
    switch (getCurrentToken()->type)
    {
    case TokenType::LeftBrace:
      {
        return parseObject();
      }
    case TokenType::String:
      {
        std::string str=getCurrentToken()->content;
        current_index++;
        return JsonValue(str);
      }
    case TokenType::LeftBracket:
      return parseArray();
    case TokenType::Number:
      {
        double number= std::stod(getCurrentToken()->content);
        current_index++;
        return JsonValue(number);
      }
    case TokenType::Bool:
      {
        bool b;
        if (getCurrentToken()->content=="true") b=true;
        else b=false;
        current_index++;
        return JsonValue(b);
      }
    case TokenType::Null:
      current_index++;
      return JsonValue();
    }
  }

};