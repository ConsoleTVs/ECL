# Erik's Configuration Language
It's my own personal taste on a configuration language. Built to be:

- Easy to parse
- Easy to write
- Low number of bytes needed.
- Transpilation to other formats like JSON, TOML, YAML.

## Example
- ECL:
```
name "my_app"
year 2019
version 1.000000
debug? y
authors [
  "Erik Campobadal <soc@erik.cat>"
  "Some Other <some@example.com>"
]
connection {
  host "127.0.0.1"
  database "sample"
  user "root"
  password "secret"
}
```

- JSON:
```
{
  "name": "my_app",
  "year": 2019,
  "version": 1.000000,
  "debug": true,
  "authors": [
    "Erik Campobadal <soc@erik.cat>",
    "Some Other <some@example.com>"
  ],
  "connection": {
    "host": "127.0.0.1",
    "database": "sample",
    "user": "root",
    "password": "secret"
  }
}
```
