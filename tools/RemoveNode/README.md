# RemoveNode 2.0

This is a experimental new take on the Foundry Nuke `Remove Node`, the issue I have with nukes current Remove Node is that you are limited to 4 Channels at a given time, so I thought it would be fun to create one where you have unlimited access to the channels you want to remove in a single node with the similar functionality as the current Remove node.

---

Current WIP node layout `This is subject to change and improvements`

![image](https://github.com/user-attachments/assets/1d37ed1f-5a04-470a-932e-08013c25f036)


# Requirments
- cmake 3.10 or later
- Nuke 15.x (`built against this version`)
- Qt Binaries ( Download [here](https://www.foundry.com/products/nuke-family/developers) )
    - Download the Binary for your OS and Nuke version, not the source file link.
 

# Build
```
git clone https://github.com/Shamiel2/Nuke_Nodez.git
```
` NOTE: In the CMakeLists you will need to change the `Qt5_DIR` to where you downloaded yours, the default path is "./Qt/lib/cmake/Qt5" `

```
cd Nuke_Nodez/tools/RemoveNode
mkdir build && cd build
cmake ..
cmake --build . --config Release
```
