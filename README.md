# Chimp

I plan to build a 2.5D 2-player tower defense game inspired by Bloons TD Battles. Each player has their own map, towers, enemies, and can spend currency to send additional enemies to their opponent.

The engine will be specialised for creating multiplayer and tower defense games, though it will not be impossible to build other software using the engine.
The engine will primarily be developed for 64-bit windows, though will be designed such that additional platforms will not require modifying the engine's API.

I plan to use these libraries:
OpenGL (modern version) - OpenGL is a low level cross-platform graphics library that will allow me to render the game in a 3 dimensional space. OpenGL will also allow me to efficiently render large amounts of enemies without thousands of draw calls. Additionally, I already have experience writing abstractions for OpenGL.
STB - STB is a collection of single-file libraries, I primarily plan to use stb_image to handle texture loading.
assimp - assimp is a library that efficiently loads 3d models and supports a wide variety of file formats.
ImGui - ImGui is a light-weight UI library primarily intended for debugging tools. UI will not be a focus of the engine so ImGui is sufficient.
ENet - ENet is a light-weight networking library that allows for sending and receiving packets of data in a consistent order. ENet also handles networking aspects such as "keep-alive" packets, having clients acknowledge packets, and splitting large packets into smaller ones and reassembling them when received.
GLFW - GLFW is a high-level cross-platform window library that allows for creating windows and receiving messages without directly interacting with the OS's API.
FLECS - FLECS is a highly efficient ECS that will be required for handling large amounts of enemies and projectiles. Additionally, the engine will primarily use components for organisation over other patterns such as OOP.
FMOD - FMOD is a library that allows for the loading and playing of sound files. FMOD additionally allows for streaming larger sound files (e.g. music) from disk.

I mainly wish to learn networking in this project, and to create a basic binary serialisation system and event system. To ensure a reasonable scope, I plan to have a server and two clients which the server trusts. This does open up vulnerabilities for cheats which e.g modify a client and prevent "lose life" packets from being sent. I do not plan to tackle this issue. Players will also simulate their opponents however the results of this simulation are just used to show the player a smooth & lag-free view, important data such as the opponent losing lives will be self-reported by the opponent and forwarded by the server to the player.
Example usage of the serialisation:
```
struct SomeStruct {
  int a;
  int b;

  static std::unique_ptr<SomeStruct> Read(const Data &data) {
    ASSERT(Types::GetId<SomeStruct> == data.GetTypeId());
    SomeStruct s;
    s.a = data.ReadInt();
    s.b = data.ReadInt();
  }

  void Write(Data &data) {
    data.WriteInt(a);
    data.WriteInt(b);
  }
}

Game() {
  Types::RegisterSerialisableType<SomeStruct>(SomeStruct::Read, SomeStruct::Write);
}
```

I also plan to build an abstraction around OpenGL. Application developers may use an ECS to render entities like so:
```
Game() {
  int id = m_AssetManager.LoadTexture("player.png");
  int entityId = m_ECS.Create();
  m_ECS.AddComponent<SpriteComponent>({ id });
}

void Render(const Renderer &renderer) {
  m_ECS.GetComponents<SpriteComponent>([&](SpriteComponent &comp) {
    renderer.RenderTexturedQuad(comp.textureId); // maybe using a transform component too?
  });
}
```

I plan to use some multithreading for certain aspects of the engine. (such as networking and asset loading!)
