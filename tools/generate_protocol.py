import json
import os
from typing import Dict, Any

def generate_rust_types(protocol: Dict[str, Any], output_path: str):
    with open(output_path, 'w') as f:
        f.write("""use serde::{Deserialize, Serialize};
use std::collections::HashMap;

""")
        
        # Generate base types
        f.write("""#[derive(Debug, Clone, Serialize, Deserialize)]
pub struct Vector3 {
    pub x: f32,
    pub y: f32,
    pub z: f32,
}

""")

        # Generate message types
        messages = protocol.get("messages", {})
        for msg_name, msg_def in messages.items():
            struct_name = "".join(word.capitalize() for word in msg_name.split("_"))
            f.write(f"#[derive(Debug, Clone, Serialize, Deserialize)]\n")
            f.write(f"pub struct {struct_name} {{\n")
            
            for prop_name, prop_def in msg_def.get("properties", {}).items():
                rust_type = get_rust_type(prop_def)
                f.write(f"    pub {prop_name}: {rust_type},\n")
            
            f.write("}\n\n")

        # Generate command types
        f.write("#[derive(Debug, Clone, Serialize, Deserialize)]\n")
        f.write("pub enum Command {\n")
        
        commands = protocol.get("commandTypes", {})
        for cmd_name, cmd_def in commands.items():
            struct_fields = []
            for prop_name, prop_def in cmd_def.get("properties", {}).items():
                rust_type = get_rust_type(prop_def)
                struct_fields.append(f"{prop_name}: {rust_type}")
            
            f.write(f"    {cmd_name} {{ {', '.join(struct_fields)} }},\n")
        
        f.write("}\n")

def generate_cpp_types(protocol: Dict[str, Any], output_path: str):
    with open(output_path, 'w') as f:
        f.write("""#pragma once

#include "CoreMinimal.h"
#include "RTSNetworkTypes.generated.h"

""")

        # Generate base types
        f.write("""USTRUCT(BlueprintType)
struct FRTSVector3
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float X;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Y;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Z;
};

""")

        # Generate message types
        messages = protocol.get("messages", {})
        for msg_name, msg_def in messages.items():
            struct_name = "FRTS" + "".join(word.capitalize() for word in msg_name.split("_"))
            f.write(f"USTRUCT(BlueprintType)\n")
            f.write(f"struct {struct_name}\n{{\n")
            f.write("    GENERATED_BODY()\n\n")
            
            for prop_name, prop_def in msg_def.get("properties", {}).items():
                cpp_type = get_cpp_type(prop_def)
                f.write(f"    UPROPERTY(EditAnywhere, BlueprintReadWrite)\n")
                f.write(f"    {cpp_type} {prop_name.capitalize()};\n\n")
            
            f.write("};\n\n")

        # Generate command types
        f.write("UENUM(BlueprintType)\n")
        f.write("enum class ERTSCommandType : uint8\n{\n")
        
        commands = protocol.get("commandTypes", {})
        for cmd_name in commands.keys():
            f.write(f"    {cmd_name},\n")
        
        f.write("};\n")

def get_rust_type(prop_def: Dict[str, Any]) -> str:
    type_name = prop_def.get("type")
    if type_name == "number":
        return "f32"
    elif type_name == "integer":
        return "i32"
    elif type_name == "string":
        return "String"
    elif type_name == "array":
        item_type = get_rust_type(prop_def["items"])
        return f"Vec<{item_type}>"
    elif type_name == "object":
        return "HashMap<String, serde_json::Value>"
    return "String"  # Default to String for unknown types

def get_cpp_type(prop_def: Dict[str, Any]) -> str:
    type_name = prop_def.get("type")
    if type_name == "number":
        return "float"
    elif type_name == "integer":
        return "int32"
    elif type_name == "string":
        return "FString"
    elif type_name == "array":
        item_type = get_cpp_type(prop_def["items"])
        return f"TArray<{item_type}>"
    elif type_name == "object":
        return "TMap<FString, FString>"  # Simplified for example
    return "FString"  # Default to FString for unknown types

def main():
    protocol_path = "../protocol/protocol.json"
    rust_output = "../server/src/protocol_gen.rs"
    cpp_output = "../client/Plugins/RTSNetwork/Source/RTSNetwork/Public/RTSNetworkTypes.h"

    with open(protocol_path) as f:
        protocol = json.load(f)

    os.makedirs(os.path.dirname(rust_output), exist_ok=True)
    os.makedirs(os.path.dirname(cpp_output), exist_ok=True)

    generate_rust_types(protocol, rust_output)
    generate_cpp_types(protocol, cpp_output)

if __name__ == "__main__":
    main()