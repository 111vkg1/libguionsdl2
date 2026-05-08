![logo](https://imgfy.ru/ib/pmZIujYaQ1P2YFN_1778274443.webp)

# **libguionsdl2 (GOS)**
libguionsdl2 (GOS or guionsdl) is
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
- Get any element by position:
```
isButtonActive = maingui.GetElementAt(50, 50)->Active;
// Get element with position near x=50, y=50 and check is it active
```
- Erase any element by Name or Id:
```
maingui.EraseElmentByName("Button");
// or
maingui.EraseElementById(0);
```
- Draw elements, thats stored:
```
maingui.Draw(renderer); // SDL_Renderer *
```
- Update elements logic:
```
maingui.Update(x, y, nulltpr); // Mouse x, mouse y, Uint32 event
```
- Get selected now element:
```
auto selected = maingui.GetSelected(); // Return pointer to GOS_Element
```

### GOS_Color
- Stores a r, g, b, a color canals
- Methods:
```
Color.SetAllTo(255); // Set r, g, b to 255

Color.SetRandom(); // Set r, g, b to random value

Color.GetHex(); // Returns string with hex color value
```

### GOS_Element
- Base interface element
- Stores position, size, color, name, text and id
- Methods:
- Define USETEXT to use text elements
```
GOS_Element *button = new GOS_Element();

button->MouseOn(x, y); // Returns true or false
button->Draw(renderer); // SDL_Renderer *
button->SetColor(color); // SDL_Color *
button->SetFace(rect); // SDL_Rect *
button->GetName(); // Returns name of object
```

### GOS_Button && GOS_StyledButton
- Inherits GOS_Element
- Overrides:
    - Draw() method shade if active
    - MouseOn() method now save result to Active
    - GOS_StyledButton have a border

### GOS_Box && GOS_StyledBox
- Inherits GOS_Element
- Overrites:
    - GOS_StyledBox have a border

## USETEXT GOS_Elements
### GOS_PointButton
- Inherits GOS_StyledButton
- If active -> Text is "v"
- If inactive -> Text is "x"

### GOS_TextBox
- Inherits GOS_Box
- Simple box with text
- To edit text:
```
TextBox->Text = "New text"
```

### GOS_TextInputBox
- Inherits GOS_Button
- Need a id to init
```
GOS_TextInputBox *tib = new GOS_TextInputBox(1); // Replace 1 with id that need
std::cout << tib->Name; // InputTextBox1
std::cout << tib->Id; // 1
```
