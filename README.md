# Federal Register API

This is an api that can return values from differnt endpoints in the federal register

given specific inputs and values, it downloads the .json file at the endpoint and parses it with 
the user given feilds.

It doesnt download with any `?=` because those feilds are found in the json file

# How to use

The struct call `EndPoints` has a few elements that you can set to get to the requested endpoint <br>

You need to set those and then call get_urls, which takes three values, the struct, a catigory, and an elementName for the json

# installing

run `make static install-static` or `make dynamic install-dynamic`

If you dont know just run the dynamic one, static one is a little faster but it makes no differnce
