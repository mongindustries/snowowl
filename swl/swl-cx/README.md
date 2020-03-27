## SnowOwl: Core

Defines the core components used by all snowOwl: modules.

### Contents

\* denotes GPU friendliness.

#### Common Data Structures

- `Point2D`\*, `Point3D`\*
- `Rect`\*, `Edge`\*
- `Path`

#### Math Data Structures / Operations

- `Vector2`\*, `Vector3`\*, `Vector4`\*
- `Matrix3`\*, `Matrix4`\*
- Projection operations
- Basic bounds check
- Basic point check

#### File Operations

- Directory organization
- File read/write

#### Memory Allocation

- Memory pool\*
- Memory Ownership model\* (similar to rust)

#### Environment Information

- Operating system name/version
- Device metrics (screen info, desktop configuration)
