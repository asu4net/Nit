#entity SpaceShip 15165415094485934680
#component TransformComponent
{
    "Position": {
        "x": 0.24714435636997223,
        "y": -2.0868148803710938,
        "z": 0.0
    },
    "Rotation": {
        "x": 0.0,
        "y": -0.0,
        "z": 0.0
    },
    "Scale": {
        "x": 1.0,
        "y": 1.0,
        "z": 1.0
    }
}
#component SpriteComponent
{
    "IsVisible": true,
    "SortingLayer": 0,
    "SpriteAssetRef": {
        "AssetId": {
            "IdHandler": 5453174695345645786
        }
    },
    "UseSubsprite": true,
    "SubSpriteName": "SpaceShip",
    "Size": {
        "x": 1.0,
        "y": 1.0
    },
    "TintColor": {
        "r": 1.0,
        "g": 1.0,
        "b": 1.0,
        "a": 1.0
    },
    "FlipX": false,
    "FlipY": false,
    "UVScale": {
        "x": 1.0,
        "y": 1.0
    }
}
#entity SceneCamera 17242001385252661092
#component TransformComponent
{
    "Position": {
        "x": 0.0,
        "y": 0.0,
        "z": 0.0
    },
    "Rotation": {
        "x": 0.0,
        "y": 0.0,
        "z": 0.0
    },
    "Scale": {
        "x": 1.0,
        "y": 1.0,
        "z": 1.0
    }
}
#component CameraComponent
{
    "IsStartCamera": true,
    "CameraProjection": "Orthographic",
    "Size": 3.0,
    "Fov": 90.0,
    "NearPlane": 0.0,
    "FarPlane": 1000.0
}
#entity MissileTemplate 927240651615057375
#component TransformComponent
{
    "Position": {
        "x": -0.7040497660636902,
        "y": -0.09452328085899353,
        "z": 0.0
    },
    "Rotation": {
        "x": 0.0,
        "y": -0.0,
        "z": 0.0
    },
    "Scale": {
        "x": 0.5,
        "y": 0.5,
        "z": 0.5
    }
}
#component SpriteComponent
{
    "IsVisible": false,
    "SortingLayer": 0,
    "SpriteAssetRef": {
        "AssetId": {
            "IdHandler": 5453174695345645786
        }
    },
    "UseSubsprite": true,
    "SubSpriteName": "Missile",
    "Size": {
        "x": 1.0,
        "y": 1.0
    },
    "TintColor": {
        "r": 1.0,
        "g": 1.0,
        "b": 1.0,
        "a": 1.0
    },
    "FlipX": false,
    "FlipY": false,
    "UVScale": {
        "x": 1.0,
        "y": 1.0
    }
}
