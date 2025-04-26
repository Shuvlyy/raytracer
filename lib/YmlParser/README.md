# YML Parser

Basic YAML parser written in C++. This is an adaptation of my previous YML
parser written in C back in 2024.

## Usage

### Creation

Just call `Yml`'s constructor with the YML file you want to open.

Beware of errors!
```c
try {
    Yml yml("file.yml");
}
catch (const yml::exception::IException& exception) {
    // Handle error here
}
```

For the rest of this README, this sample YML configuration file will be used:
```yml
name: "Lysandre"
birth:
  year: 2006
  month:
    name: "July"
    index: 7
height: 1.86
gpa: 3.18
hasCramptes: false
```

### 📰 Reading content

For now, only `int`, `double`, `bool` and strings are supported.

> [!TIP]
> Every value is de facto stored as a string.

```cpp
Yml yml(...); // Initialize your YML here

int birthYear = yml["birth"]["year"].as<int>(); // 2006
double gpa = yml["gpa"].as<double>(); // 3.18
std::string name = yml["name"].as(); // "Lysandre"
/*                               ^ Note that for strings, you don't have to specify the type */
bool hasCramptes = yml["hasCramptes"].as<bool>(); // false
```

You can also get a node like this:
```cpp
std::string birthMonth = yml.getNode("birth.month.name").as<std::string>();
```

> [!WARNING]
> If an error is encountered during reading (wrong type, non-existent node), an
> exception that inherits from `yml::exception::IException` will be thrown.
> It is up to you to properly handle it!!

## Future

This parser is kind of incomplete for now, it only serves for reading purposes.
But setters and configuration saving will be soon added!

## Time spent

Around 38 hours.\
My wakatime statistics aren't representative of my real time
spent, as it also counted my time spent on the MyRPG and Raytracer project in
general.

### Made with 💜 by [Lysandre B.](https://www.github.com/shuvlyy)
