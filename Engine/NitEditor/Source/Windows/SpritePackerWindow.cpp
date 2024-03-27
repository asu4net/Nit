#include "SpritePackerWindow.h"
#include "imgui.h"
#include "Asset\Sprite.h"
#include "Asset\Content.h"
#include "Asset\AssetRef.h"
#include "ImGui\ImGuiUtils.h"

#ifdef _WIN32
#define popen  _popen
#define pclose _pclose
#endif

namespace Nit::SpritePackerWindow
{
    struct Frame { float x, y, w, h; };
    struct Size { float w, h; };

    struct SpriteData
    {
        String filename;
        Frame frame;
        bool rotated;
        bool trimmed;
        Frame spriteSourceSize;
        Size sourceSize;
    };

    struct Meta
    {
        String app;
        String version;
        String image;
        String format;
        Size size;
        int scale;
        String smartupdate;
    };

    struct Sheet
    {
        DynamicArray<SpriteData> frames;
        Meta meta;
    };

    RTTR_REGISTRATION
    {
         rttr::registration::class_<Frame>("Frame")
            .constructor<>()(rttr::policy::ctor::as_object)
            .property("x", &Frame::x)
            .property("y", &Frame::y)
            .property("w", &Frame::w)
            .property("h", &Frame::h);

        rttr::registration::class_<Size>("Size")
            .constructor<>()(rttr::policy::ctor::as_object)
            .property("w", &Size::w)
            .property("h", &Size::h);

        rttr::registration::class_<SpriteData>("SpriteData")
            .constructor<>()(rttr::policy::ctor::as_object)
            .property("filename", &SpriteData::filename)
            .property("frame", &SpriteData::frame)
            .property("rotated", &SpriteData::rotated)
            .property("trimmed", &SpriteData::trimmed)
            .property("spriteSourceSize", &SpriteData::spriteSourceSize)
            .property("sourceSize", &SpriteData::sourceSize);

        rttr::registration::class_<Meta>("Meta")
            .constructor<>()(rttr::policy::ctor::as_object)
            .property("app", &Meta::app)
            .property("version", &Meta::version)
            .property("image", &Meta::image)
            .property("format", &Meta::format)
            .property("size", &Meta::size)
            .property("scale", &Meta::scale)
            .property("smartupdate", &Meta::smartupdate);

        rttr::registration::class_<Sheet>("Sheet")
            .constructor<>()(rttr::policy::ctor::as_object)
            .property("frames", &Sheet::frames)
            .property("meta", &Sheet::meta);
    }

        String Execute(const char* cmd)
    {
        Array<char, 128> buffer;
        String result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

        NIT_CHECK(pipe, "popen() failed!");

        while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
        {
            result += buffer.data();
        }

        return result;
    }

    void Show(bool* bShow)
    {
        ImGui::Begin("Sprite Packer", bShow);

        if (Input::IsEnabled() && (ImGui::IsWindowFocused() || ImGui::IsWindowHovered()))
            Input::SetEnabled(false);

        // source
        static String source;
        ImGui::InputFolder("Source", source);

        static String dest = Nit::GetWorkingDirectory().string() + "\\" + Sprite::DefaultFolder();
        ImGui::InputFolder("Destination", dest);

        static String name;
        ImGui::InputText("Asset Name", name);

        if (ImGui::Button("Generate"))
        {
            if (source.empty() || dest.empty() || name.empty())
            {
                NIT_CHECK(false, "Missing stuff!");
                ImGui::End();
                return;
            }

            if (Content::HasAsset(name))
            {
                NIT_CHECK(false, "Asset already exist!");
                ImGui::End();
                return;
            }

            StringStream ss;
            ss << "cd ../../ThirdParty/TexturePacker && TexturePacker";
            ss << " " << source;

            String fullDest = dest + "\\" + name + ".json";

            ss << " --data " << fullDest;

            ss << " --max-width 5000 --max-height 5000";
            ss << " --trim-sprite-names";
            ss << " --format json-array"; //default output format
            ss << " --png-opt-level 0 --extrude 0 --algorithm Basic --trim-mode None --disable-auto-alias"; //free version cfg

            NIT_LOG_TRACE("Generating sprite atlas...\n");
            const String result = Execute(ss.str().c_str());
            NIT_LOG_TRACE("%s\n", result.c_str());

            if (result.empty())
            {
                NIT_CHECK(false, "Something went wrong!\n");
                ImGui::End();
                return;
            }

            std::ifstream atlasFile(fullDest);
            StringStream atlasStream;
            atlasStream << atlasFile.rdbuf();

            /*if (!std::filesystem::remove(fullDest.c_str()))
            {
                NIT_CHECK(false, "File not found!\n");
            }*/

            Sheet sheet;
            Serialization::FromJson(atlasStream.str(), sheet);

            float atlasWidth, atlasHeight;
            atlasWidth = sheet.meta.size.w;
            atlasHeight = sheet.meta.size.h;

            AssetRef spriteRef;
            AssetData spriteAssetData;
            spriteAssetData.Name = name;

            const String absolutePath = dest + "\\" + name + ".png";
            const String relativePath = std::filesystem::relative(absolutePath, Nit::GetWorkingDirectory()).string();
            spriteAssetData.Path = relativePath;
            
            spriteRef = Content::CreateAsset<Sprite>(spriteAssetData);
            Sprite& sprite = spriteRef.As<Sprite>();
            Content::TryLoadAsset(spriteRef);

            if (!spriteRef.IsValid())
            {
                NIT_CHECK(false, "Something went wrong!\n");
                ImGui::End();
                return;
            }

            for (const SpriteData& data : sheet.frames)
            {
                const Frame& frame = data.frame;
                sprite.PushSubSprite(data.filename, { frame.x, frame.y }, { frame.w, frame.h });
            }

            Content::SerializeAsset(spriteRef.Get(), dest);
        }

        ImGui::End();
    }
}

/*

Usage: TexturePacker [options] [<*.png|gif|tif|jpg|...>|<spritedir>] [<*.tps>]

You can specify one or more .png or .tps files or directories for processing.
   <*.tps>                      TexturePacker project file(s) to process.
                                Options in the file can be overwritten from the commandline
   <spritedir>                  Recursively add all known files in the sprite sheet
   <*.png|gif|tif|jpg|...>      Input sprite formats, supported:
                                     astc      Adaptive Scalable Texture Compression
                                     basis     Basis Universal Texture
                                     bmp       Bitmap
                                     dds       DirectDraw Surface
                                     gif       Graphics Interchange Format
                                     ico       Icon (Windows)
                                     jpeg      Joint Photographic Experts Group
                                     jpg       Joint Photographic Experts Group
                                     ktx       Khronos Texture (Version 1)
                                     ktx2      Khronos Texture (Version 2)
                                     pbm       Portable Bitmap
                                     pgm       Portable Gray Map
                                     pkm       PKM file format (etc1/2)
                                     png       Portable Network Graphics
                                     ppm       Netpbm color image format
                                     psd       Photoshop image format
                                     pvr       PowerVR image format
                                     pvr.ccz   PowerVR image format (zlib)
                                     pvr.gz    PowerVR image format (gz)
                                     pvrtc     PowerVR image format
                                     svg       Scalable Vector Graphics
                                     svgz      Scalable Vector Graphics
                                     tga       Targa image format
                                     tif       Tagged Image Format
                                     tiff      Tagged Image Format
                                     webp      WebP format
                                     xbm       X Bitmap
                                     xpm       X Pixmap

Options:
  --help                        Display help text
  --version                     Print version information
  --gui                         Launch in with graphical user interface

  Output:
  --sheet <filename>            Name of the sheet to write, see texture-format for formats available
  --texture-format <id>         Sets the format for the textures.
                                The format type is automatically derived from the sheet's file name
                                if possible.
                                Available formats:
                                    png               PNG (32bit)
                                    png8              PNG (8bit indexed)
                                    pvr3              PowerVR Texture Format, PVR Version 3
                                    pvr3gz            PowerVR Texture Format, PVR Version 3, compressed with gzip
                                    pvr3ccz           PowerVR Texture Format, PVR Version 3, compressed with zlib, cocos2d header
                                    jpg               JPG image format, lossy compression, no transparency
                                    bmp               24 bit BMP
                                    tga               Targa Image Format
                                    tiff              Tagged Image File Format
                                    pkm               PKM image format, ETC1 compression
                                    webp              WebP lossless / lossy format
                                    ktx               Khronos Texture (Version 1)
                                    ktx2              Khronos Texture (Version 2)
                                    astc              ASTC compressed file
                                    basis             Basis Universal compressed file
                                    dds               DirectDraw Surface

  --format <format>             Format to write, default is cocos2d
                                Available formats:
                                    agk                 Format for AppGameKit
                                    bhive               Format for BHive
                                    cegui               Format for CEGUI / OGRE
                                    cocos2d             plist format version 3 for cocos2d
                                    cocos2d-v2          old plist format version 2 for cocos2d (deprecated)
                                    cocos2d-x           plist format version 3 for cocos2d-x, Axmol and CocosCreator
                                    css-simple          Simple CSS sprite format for web design
                                    gideros             Format for Gideros
                                    json-array          Text file for json/html as array
                                    json                Text file for json/html as hash
                                    libgdx              text file for lib GDX
                                    melonjs             Data file for melonJS
                                    moai                Format for Moai
                                    phaser-json-array   JSON array data for Phaser
                                    phaser-json-hash    JSON hash data for Phaser
                                    pixijs4             Data file for PixiJS
                                    shiva3d-jpsprite    Shiva3D with JPSprite extension
                                    slick2d             Format for Slick2D
                                    spark               Spark AR Studio (Facebook)
                                    sparrow             xml file for Sparrow/Starling SDK
                                    spine               text file for Spine
                                    spritesheet-only    Exports only the sprite sheet without data file
                                    spritekit           plist format for SpriteKit, Objective-C header file
                                    spritekit-swift     plist format for SpriteKit, with swift class file
                                    spriter             JSON file for Spriter
                                    unity               Text file for Unity(R), json format with .txt ending
                                    unreal-paper2d      Format for UnrealEngine / Paper2d
                                    vplay               JSON file for V-Play engine
                                    wave-engine-1       WaveEngine Sprite Sheet
                                    xml                 Generic XML format

  --data <filename>             Name of the data file to write

  --save <filename>.tps         Save texture settings as .tps file

  --class-file <filename>       Name of the class output file (spritekit-swift)

  --header-file <filename>      Name of the header output file (cocos2d-x, spritekit)

  --source-file <filename>      Name of the source output file (cocos2d-x)

  --force-publish               Ignore smart update hash and force re-publishing of the files
  --texturepath <path>          Adds the path to the texture file name stored in the data file.
                                Use this if your sprite sheets are not stored in another folder than your data files.
  --trim-sprite-names           Removes .png, .bmp and .jpg from sprite names
  --prepend-folder-name         Adds the smart folders name to the sprite names
  --replace <regexp>=<string>   Replaces matching parts of the sprite's name with <string>
                                Uses full regular expressions, make sure to escape the expression
  --ignore-files <wildcard>     Ignores all images fitting the given pattern (may be used several times)
                                You can use * and ?, make sure to escape the wildcards when working with bash

  Scaling variants:
  --variant <expr>              Adds a scaled variant of the sheet. Format of the expr
                                <scale>:<name>[:<filter>[:allowfraction][:<width>:<height>]]
                                   <scale>          floating point value, e.g. 0.5
                                   <name>           name of the variant, used to replace {v} in file names, e.g. @2x
                                   <filter>         only sprites which match this filter will be added to the variant
                                   allowfraction    allow floating point values for this scaling if no common
                                                    base factor can be calculated (force identical layout)
                                   <width>:<height> optional maximum size of the texture, if not set the
                                                    maximum texture size will be used (default: 2048x2048)
  --force-identical-layout      Preserves the layout across multiple scaling variants
                                Might require enabling allowfraction on some variants if no common
                                base factor can be derived

  Algorithm settings:
  --algorithm <name>            Choose algorithm
                                    MaxRects        Powerful packing algorithm (extended)
                                    Grid            Places sprites on regular grid, the largest sprite determines the cell size
                                    Basic           Simple algorithm for tilemaps and atlases (free)
                                    Polygon         Tight polygon packing
    MaxRects
      --maxrects-heuristics     Heuristic for MaxRects algorithm
                                    Best              Tests all available placements and uses the result with the least used space
                                    ShortSideFit      ShortSideFit
                                    LongSideFit       LongSideFit
                                    AreaFit           AreaFit
                                    BottomLeft        BottomLeft
                                    ContactPoint      ContactPoint

    Polygon
      --align-to-grid <int>     Ensures that the top-left corners of the untrimmed sprites are placed on
                                sprite sheet coordinates divisible by the passed value

    Basic
      --basic-sort-by           Sort order for the sprite list
                                    Best              Tests all sorting variants and uses the result with the least used space
                                    Name              Sorts sprites by name
                                    Width             Sorts sprites by width
                                    Height            Sorts sprite by height
                                    Area              Sorts sprites by their area (width*height)
                                    Circumference     Sorts sprites by their circumference (width+height)
      --basic-order             Sorting direction
                                    Ascending         Sorts values from small to large
                                    Descending        Sorts values from large to small


  Dimensions and layout:
  --width <int>                 Sets fixed width for texture
  --height <int>                Sets fixed height for texture
  --max-width <int>             Sets the maximum width for the texture in auto size mode, default is 2048
  --max-height <int>            Sets the maximum height for the texture in auto size mode, default is 2048
  --max-size <int>              Sets the maximum width and height for the texture in auto size mode, default is 2048
  --size-constraints <value>    Restrict sizes
                                    POT               Power of 2 (2,4,8,16,32,...)
                                    WordAligned       Texture width is multiple of 2 (for 16-bit formats)
                                    AnySize           Any size
  --force-squared               Force squared texture
  --pack-mode <mode>            Optimization mode: Fast, Good, Best
  --multipack                   Create multiple sprite sheets if not all sprites match into a single one
  --common-divisor-x <int>      Resizes sprites - widths will be divisible by this value
  --common-divisor-y <int>      Resizes sprites - heights will be divisible by this value
  --default-pivot-point <x>,<y> Sets default pivot point used for sprites passed on command line

  Padding and rotation:
  --shape-padding <int>         Sets a padding around each shape, value is in pixels
  --border-padding <int>        Sets a padding around each the border, value is in pixels
  --padding <int>               Sets a padding around each shape, and to the border, value is in pixels
  --enable-rotation             Enables rotation of sprites (overriding file format's defaults)
  --disable-rotation            Disables rotation of sprites (overriding file format's defaults)
  --trim-mode <value>           Remove transparent parts of a sprite to shrink atlas size and speed up rendering
                                    None              Keep transparent pixels
                                    Trim              Remove transparent pixels, use original size.
                                    Crop              Remove transparent pixels, use trimmed size, flush position.
                                    CropKeepPos       Remove transparent pixels, use trimmed size, keep position.
                                    Polygon           Approximate sprite contour with polygon path.
  --trim-threshold <int>        Trim alpha values under the threshold value 1..255, default is 1
  --trim-margin <int>           Transparent margin which is left over after trimming
  --tracer-tolerance <int>      Deviation of the polygon approximation from the exact sprite outline, default is 200
  --disable-auto-alias          Disables automated alias creation

  Graphics optimization:
  --opt <pixelformat>           Optimized output for given pixel formats. Supported formats are:
                                    RGBA8888           32bit, 8bit/channel, 8bit transparency
                                    RGBA4444           16bit, 4bit/channel, 4bit transparency
                                    RGBA5551           16bit, 5bit/channel, 1bit transparency
                                    RGBA5555           20bit, 5bit/channel, 5bit transparency
                                    BGRA8888           32bit, 8bit/channel, 8bit transparency
                                    RGB888             24bit, 8bit/channel, no transparency
                                    RGB565             16bit, 5bit red, 6bit green, 5bit blue, no transparency
                                    PVRTCI_2BPP_RGBA   PVRTC compression, 2bit per pixel
                                    PVRTCI_4BPP_RGBA   PVRTC compression, 4bit per pixel
                                    PVRTCI_2BPP_RGB    PVRTC compression, 2bit per pixel
                                    PVRTCI_4BPP_RGB    PVRTC compression, 4bit per pixel
                                    PVRTCII_2BPP       PVRTC2 compression, 2bit per pixel
                                    PVRTCII_4BPP       PVRTC2 compression, 4bit per pixel
                                    ETC1_RGB           ETC1 compression
                                    ETC1_A             ETC1 Alpha channel only
                                    ETC2_RGB           ETC2 compression
                                    ETC2_RGBA          ETC2 Alpha
                                    DXT1               Compressed with DXT1, 1 bit transparency
                                    DXT5               Compressed with DXT5, transparency
                                    ASTC_4x4           Compressed with ASTC 4x4 (8.00 bits/pixel)
                                    ASTC_5x4           Compressed with ASTC 5x4 (6.40 bits/pixel)
                                    ASTC_5x5           Compressed with ASTC 5x5 (5.12 bits/pixel)
                                    ASTC_6x5           Compressed with ASTC 6x5 (4.27 bits/pixel)
                                    ASTC_6x6           Compressed with ASTC 6x6 (3.56 bits/pixel)
                                    ASTC_8x5           Compressed with ASTC 8x5 (3.20 bits/pixel)
                                    ASTC_8x6           Compressed with ASTC 8x6 (2.67 bits/pixel)
                                    ASTC_8x8           Compressed with ASTC 8x8 (2.00 bits/pixel)
                                    ASTC_10x5          Compressed with ASTC 10x5 (2.56 bits/pixel)
                                    ASTC_10x6          Compressed with ASTC 10x6 (2.13 bits/pixel)
                                    ASTC_10x8          Compressed with ASTC 10x8 (1.60 bits/pixel)
                                    ASTC_10x10         Compressed with ASTC 10x10 (1.28 bits/pixel)
                                    ASTC_12x10         Compressed with ASTC 12x10 (1.07 bits/pixel)
                                    ASTC_12x12         Compressed with ASTC 12x12 (0.89 bits/pixel)
                                    BASISU_ETC1S       Compressed with BasisUniversal ETC1S
                                    BASISU_UASTC       Compressed with BasisUniversal UASTC
                                    ALPHA              8bit transparency
                                    ALPHA_INTENSITY    8bit intensity, 8bit transparency
  --dither-type <dithertype>    Dithering to improve quality of color reduced images
                                    NearestNeighbour      no dithering
                                    Linear                no dithering
                                    FloydSteinberg        Floyd Steinberg, no alpha
                                    FloydSteinbergAlpha   Floyd Steinberg, with alpha
                                    Atkinson              Atkinson, no alpha
                                    AtkinsonAlpha         Atkinson, alpha
                                    PngQuantLow           PNG-8 only: minimum dithering
                                    PngQuantMedium        PNG-8 only: medium dithering
                                    PngQuantHigh          PNG-8 only: strong dithering
  --background-color <rrggbb>   Set solid background color, default is none, which is transparent
                                The value is a tuple of 3 hexadezimal digit pairs, each pair represents
                                a color channel in order red, green, blue, E.g. ff0000 for red, ffffff for white
  --jpg-quality <value>         Sets the quality for jpg export: -1 for default, 0..100 where 0 is low quality
  --astc-quality <value>        Set quality for ASTC codecs, (0=lowest quality, ... 4=best quality], default is 2
  --basisu-quality <value>      Set quality for Basic Universal codecs, (0=lowest quality, ... 4=best quality), default is 2
  --dxt-mode <value>            DXT1/5 compression mode: linear, perceptual
  --etc1-quality <value>        Set quality for ETC1 compression: (0=lowest quality,...100=best quality), 70 is default
  --etc2-quality <value>        Set quality for ETC2 compression: (0=lowest quality,...100=best quality), 70 is default
  --pvr-quality <value>         Set quality for PVRTC codecs, (0=lowest quality, ... 7=best quality), default is 3
  --flip-pvr                    Flips PVR files vertically (used for unity framework)
  --content-protection <key>    Content protection: Encrypt pvr.ccz files.
                                Key: 128 bit, 32 hex digits [A-F0-9]
  --alpha-handling <value>      Defines how color values of transparent pixels are processed:
                                    KeepTransparentPixels    Transparent pixels are copied from sprite to sheet without any change
                                    ClearTransparentPixels   Color values of transparent pixels are set to 0 (transparent black)
                                    ReduceBorderArtifacts    Transparent pixels get color of nearest solid pixel
                                    PremultiplyAlpha         All color values are multiplied with the alpha values
  --dpi <value>                 Set dpi for output image (default is 72)
  --heuristic-mask              Removes equal colored pixels from the border of sprites. Creating a transparency mask
                                on sprites which use one unique color as transparent color.
  --png-opt-level <value>       Optimization level for pngs (0=off, 1=use 8-bit, 2..7=png-opt)
  --webp-quality <value>        Quality level for WebP format (0=low, 100=high, >100=lossless), default is lossless

  --extrude <int>               Extrudes the sprites by given value of pixels to fix flickering problems in tile maps
  --scale <float>               Scales all images before creating the sheet. E.g. use 0.5 for half size
  --scale-mode <mode>           Use mode for scaling:
                                    Smooth            Smooth
                                    Fast              Fast (Nearest Neighbor)
                                    Scale2x           Scale2x (fixed 2x upscaling)
                                    Scale3x           Scale3x (fixed 3x upscaling)
                                    Scale4x           Scale4x (fixed 4x upscaling)
                                    Eagle             Eagle2x (fixed 2x upscaling)
                                    Hq2x              Hq2x (fixed 2x upscaling)

  Normal maps:
  --pack-normalmaps             Packs normal maps on separate sheet, with same layout as sprites
  --normalmap-filter <string>   Sprites with file paths containing this string are treated as normal maps
  --normalmap-suffix <string>   Suffix which is added to a sprite name to find the corresponding normal map
  --normalmap-sheet <filename>  File to which the normal map sprite sheet is written

  Custom Exporters
  --custom-exporters-directory <dirname> Search for additional custom exporters in this folder


  Debugging:
  --shape-debug                 Creates boxes around shapes for debugging
  --verbose                     Be verbose
  --quiet                       No output except for errors

  License management:
  --license-info                Prints information about the currently installed license
  --activate-license <key>      Activate a license key; floating licenses will automatically be deactivated on exit
  --deactivate-license          Deactivate current license on this machine


Examples:

  TexturePacker sheet.tps
        Update sprite sheets set in the sheet.tps project file

  TexturePacker template.tps sprites --data sheet.plist --sheet sheet.png
        Import options from the template.tps file, import sprites from the sprites folder
        write sheet.plist and sheet.png
  TexturePacker assets
        pack the assets folder with default settings: Creates out.plist and out.png from all png
        files in the 'assets' directory

  TexturePacker --data main-hd.plist --format cocos2d --sheet main-hd.png assets
        same as above, but with output files main-hd.plist, main-hd.png

  TexturePacker --scale 0.5 --max-size 1024 --data main.plist --format cocos2d --sheet main.png assets
        creates main.plist and main.png from all files in assets
        scaling all images to 50%, trimming all files and creating
        a texture with max. 1024x1024px

  TexturePacker --variant 1:-hd --variant 0.5: --data main{v}.plist --sheet main{v}.png --format cocos2d assets
        combines the two TexturePacker calls shown above: it creates variants with scaling
        factors 1.0 and 0.5, and stores them in main.plist/png and main-hd.plist/png
        ({v} is replaced by variant name)

  TexturePacker --variant 1:-hd:@2x --variant 1: --data main{v}.plist --sheet main{v}.png --format cocos2d assets
        similar to the example before, but TexturePacker expects that you already provide sprites
        named like 'a.png', 'b.png', 'a@2x.png', 'b@2x.png'. It selects the '@2x' sprites and put them
        on the 'main-hd' sprite sheet, and the other ones on the 'main' sprite sheet.

  TexturePacker --data main.atlasc --format spritekit assets
        the SpriteKit exporter uses variant filters by default, and puts sprites with the following suffixes
        to separate sprite sheets:
        '~iphone', '~ipad', '@2x~iphone', '@2x~ipad', '@2x', '@3x~iphone', '@3x', all remaining sprites

*/