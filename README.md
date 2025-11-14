## Air Traffic Control Tower Software Project Details

### 1. Project Identification and Objective

The project is titled **Air Traffic Control Tower Software**.

**Objective :** The primary goal is for the software to receive data from the **radar system** concerning all flying objects (including airplanes, fighter jets, and helicopters) moving over the country's airspace. The software must extract the necessary information and display this data in a **user-friendly** manner. Displaying vehicle location (car, bird, or similar) on a map for guidance and control is noted as a common and useful software component in navigation systems.

### 2. Technical Implementation and Stack

**Technology:** The project utilizes **Qt & QML**. Specifically, the implementation will use **Qt 6**.
**Development Environment:** The system development environment will be the **Windows operating system**.

**Implementation Steps :**

1. Reviewing the **Model in Qt** and connecting it to the **View** to display a dynamic list of data.
2. Covering the basics of QML and the **`Map Qml Type`**.
3. Running the QML code inside a **`Widget`**.
4. **Data Flow:** Data is received from the network, information is extracted according to the protocol, provided to the `model`, and the `view` is updated.

### 3. Requirements and Communication Protocol

#### Radar Communication (1-3)

The software connects to the radar using **network communication** and the **TCP protocol**. Messages are constantly being sent from the radar to the Air Traffic Control software.

**Byte Order and Filtering:**

* The system uses **little-endian** byte order, meaning the least significant byte (LSB) is placed at the lower index. For a 4-byte unsigned value (Uint), the LSB is at index 0 and the most significant byte (MSB) is at index 3.
* The software must only process data packets that start with the specific **Header** and end with the **Footer**. Any other unknown data will be discarded.
  * Header hex value: `A5 A5 A5 A5`.
  * Footer hex value: `55 55 55 55`.

**Coordinate Calculation:**

* The actual `latitude` value required for map display is obtained by dividing the received `Latitude` value by the **`Latitude Factor`**.
* The actual `longitude` value is obtained by dividing the received `Longitude` value by the **`Longitude Factor`**.

#### Graphical User Interface (GUI) (2-3)

While a wireframe is suggested for clarification, the final GUI appearance can vary based on the designer’s preference. A **beautiful and creative graphic** will result in higher scores.

The GUI must consist of **3 main sections**:

1. **Left List:** Displays all flying objects with a brief description.
2. **Map:** Displays the current location of every flying object.
3. **Right Text Area:** Provides detailed explanations for a specific selected flying object.

When a user selects an object from the left list, that object must be **highlighted** (برجسته مشخص) on the map, and its detailed information must be presented in the right text area.

### 4. Data Structure and Message Fields (Table 1)

The message format uses little-endian byte order. The message length is determined by the fields listed below (approximately 39 bytes, indices 0-38).

| Name | Description | Length (byte) | Byte Index | Byte Order | Data Type/Note |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Header** | 0xA5 0xA5 0xA5 0xA5 | 4 | 0–3 | 0 | - |
| **Type ID** | Uint8—According to Table 2 | 1 | 4 | 0 | Uint8 |
| **Source Airport ID** | Uint8—According to Table 3 | 1 | 5 | 0 | Uint8 |
| **Destination Airport ID** | Uint8—According to Table 3 | 1 | 6 | 0 | Uint8 |
| **latitude** | - | 4 | 7–10 | 0–3 | Uint32 |
| **Latitude factor** | - | 2 | 11–12 | 0–1 | Uint16 |
| **Longitude** | - | 4 | 13–16 | 0–3 | Uint32 |
| **Longitude factor** | - | 2 | 17–18 | 0–1 | Uint16 |
| **Altitude** | - | 4 | 19–22 | 0–3 | Uint32 |
| **Flight ID** | - | 4 | 23–26 | 0–3 | Uint32 |
| **Reserved for future** | Reserve | 4 | 27–30 | - | - |
| **Reserved for future** | Reserve | 4 | 31–34 | - | - |
| **Footer** | 0x55 0x55 0x55 0x55 | 4 | 35–38 | 0–3 | - |

***

### 5. Type IDs and Aircraft Details (Table 2)

Table 2 specifies various aircraft types, their capacities, range, and corresponding Type IDs.

| Type ID | Name | Type | Capacity | Range (nautical mile) |
| :--- | :--- | :--- | :--- | :--- |
| **0xa1** | Boing 747-8 | Commercial Airplane | 660 | 8000 |
| **0xa2** | Airbus A380 | Commercial Airplane | 853 | 8000 |
| **0xb1** | Bell 206 | Helicopter | 6 | 374 |
| **0xb2** | Lockheed Martin F-35 Lightning II | Military jet | 1 | 1188 |
| **0xb3** | Airbus A320neo | Commercial Airplane | 194 | 4000 |
| **0xb4** | Boing 737 MAX | Commercial Airplane | 204 | 3850 |
| **0xb5** | Airbus A220 | Commercial Airplane | 150 | 3400 |
| **0xb6** | Airbus A321XLR | Commercial Airplane | 220 | 4700 |
| **0xb7** | Boeing 787 Dreamliner | Commercial Airplane | 330 | 7530 |
| **0xc1** | Airbus A350 | Commercial Airplane | 410 | 8700 |
| **0xd1** | Boeing 777 | Commercial Airplane | 396 | 7250 |
| **0xd2** | Boeing 777X | Commercial Airplane | 426 | 7285 |
| **0xd5** | Airbus A380 | Commercial Airplane | 615 | 8000 |
| **0xe1** | Robinson R44 | Helicopter | 4 | 300 |
| **0xe8** | Airbus H125 | Helicopter | 6 | 320 |
| **0xe2** | Bell 407 | Helicopter | 7 | 324 |
| **0xf5** | Leonardo AW139 | Helicopter | 17 | 540 |
| **0xf0** | Lockheed Martin F-22 Raptor | Military Jet | 1 | 1600 |
| **0xf1** | Chengdu J-20 Mighty Dragon | Military Jet | 1 | 1100 |
| **0xf2** | Sukhoi Su-57 Felon | Military Jet | 1 | 2000 |

***

### 6. Airport Identifiers (Table 3)

Table 3 lists the assigned Airport IDs for various Source and Destination airports.

| Airport ID | Name | Country |
| :--- | :--- | :--- |
| **0x1a** | Imam Khomeini | Iran |
| **0x1b** | Shahid Beheshti | Iran |
| **0x1c** | Muscat | Oman |
| **0x1d** | Istanbul | Turkey |
| **0x1e** | Shiraz | Iran |
| **0x1f** | King Fahd | Saudi Arabia |
| **0x3a** | Dubai | United Arab Emirates |
| **0xa2** | Hamad | Qatar |
| **0x10** | Kuwait | Kuwait |
| **0x2b** | Erbil | Iraq |
| **0x3c** | Luxor | Egypt |

### 7. Software Testing and Delivery

To test the developed software, radar data will be sent to the system from another system. The final software must be **robust and secure** (مقاوم و ایمن) against:

* Incorrect data.
* Incorrect protocols.
* High volumes of incoming data.

Upon completion, the project requires the submission of the **code** and the **deployed Windows program**.
