# **libguionsdl2 (GOS)**
libguionsdl2 (GOS or guionsld) is
GUI header library for SDL2. 
Requares SDL2_ttf, SDL2, SDL2_image

## GOS has a simple structure:
### GOS_GUI
- Stores all GUI elements
- Can get any element by Name:
```
GOS_GUI maingui;

bool isButtonActive = maingui.GetElementByName("Button")->Active;
// Get element named "Button" and check is it active
```
- Can get any element by position:
```
isButtonActive = maingui.GetElementAt(50, 50)->Active;
// Get element with position near x=50, y=50 and check is it active
```
- Can erase any element by Name or Id:
```
maingui.EraseElmentByName("Button");
// or
maingui.EraseElementById(0);
```

