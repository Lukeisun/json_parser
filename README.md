# JSON parser
[Saw this tweet](https://x.com/JohnBurton/status/1875147123148324977) and felt inspired/challenged/fearful

Would maybe like to make this into a library just for fun

Would not recommend using this ever :D

## Building

- Include the hpp files and add the [Lexer|Parser].cpp to your build script. I should probably make this better :)

## Usage 
```cpp
  std::filesystem::path path(argv[1]);
  JSON json(path);
  json.lex();
  Value v = json.parse();
```
Can provide a path or a string 

## TODO:
- Number extraction (right now its just a string)
- ~Overload to get Value of a Key/index a value e.g. `Value foo = json["test"]`~
- Should make it so a potential user can choose if the library should copy a buffer or reference a user provided one
- error handling
