# Federal Register API

This is an api that can return values from differnt endpoints in the federal register

given specific inputs and values, it downloads the .json file at the endpoint and parses it with 
the user given feilds.

It doesnt download with any `?=` because those feilds are found in the json file

# How to use

The struct call `EndPoints` has a few elements that you can set to get to the requested endpoint <br>

You need to set those and then call get_urls, which takes three values, the struct, a catigory, and an elementName for the json

# installing

This defaults to a static liberary because why not. run `sudo make clean install` or `doas make clean install`
