# reflect-parser-validator

```
cd build
cmake ..
make

curl -X POST http://localhost:18080/ \
     -H "Content-Type: application/json" \
     -d '{
           "id": "1222",   
           "nested": {
               "value": -1
           }
         }'
```
Reference
- https://github.com/getml/reflect-cpp
- https://github.com/ibireme/yyjson

