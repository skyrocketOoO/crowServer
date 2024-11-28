# crowServer

```
cd build
cmake ..
make

curl -X POST -H "Content-Type: application/json" -d '{"name":"Alice"}' http://localhost:18080/add_json
```