# Project flowcharts

These diagrams document the firmware's current behavior.

## Startup and time synchronization

```mermaid
flowchart TD
    A([Power on]) --> B[Initialize I2C and OLED]
    B --> C{OLED found?}
    C -- No --> X[Print error and stop]
    C -- Yes --> D[Connect to Wi-Fi]
    D --> E{Connected?}
    E -- No --> H[Run animation without date]
    E -- Yes --> F[Configure NTP]
    F --> G{Time available?}
    G -- No --> H
    G -- Yes --> I[Cache weekday and date]
    I --> J[Render display]
    H --> J
```

## Main animation loop

```mermaid
flowchart TD
    A([loop]) --> B{Wi-Fi connected and time not ready?}
    B -- Yes --> C[Configure NTP and try time read]
    B -- No --> D[Update date strings]
    C --> D
    D --> E{180 ms elapsed?}
    E -- No --> F[Short delay]
    E -- Yes --> G[Advance animation frame]
    G --> H[Redraw OLED]
    H --> F
    F --> A
```

## Display layout

The firmware draws one 128×64 bitmap frame, then overlays the weekday and date when time is available. The exact text placement is set in `drawScreen()`.
