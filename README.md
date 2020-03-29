# UE Artnet
Integrate libarnet permitting Actors to be recognized as artnetNode

# Technical Details

* Include C++ Source for C++ Projects.

# Documentation
Create an Actor, and attach a ArnetNode Component to it.

![Alt Attach](img/attachcomponent.png)

Configure the component by setting your address ip, universe (port) and sub-uni (subnet). Give it a short and long name.
Num Port is used to allow this node to handle more than one universe.
Set the port direction to Both.
![Alt Configure](img/configurecomponent.png)

This is a small graph of wath you can do with the values.
![Alt Sample](img/graphsample.png)
![Alt Demo](img/demo.gif)

Ths is the node discovered by DMX-Workshop
![Alt DMX-Workshop](img/rdm.png)

# TODO

* Write values to universes

# Support
nicosanta@brightnightgames.net
