/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   drone_badcalib_png;
    const int            drone_badcalib_pngSize = 19447;

    extern const char*   drone_disconnected_png;
    const int            drone_disconnected_pngSize = 16184;

    extern const char*   drone_error_png;
    const int            drone_error_pngSize = 15534;

    extern const char*   drone_off_png;
    const int            drone_off_pngSize = 16207;

    extern const char*   drone_ready_png;
    const int            drone_ready_pngSize = 15244;

    extern const char*   drone_warning_png;
    const int            drone_warning_pngSize = 14497;

    extern const char*   circle_png;
    const int            circle_pngSize = 14206;

    extern const char*   line_png;
    const int            line_pngSize = 6442;

    extern const char*   multiPosition_png;
    const int            multiPosition_pngSize = 5762;

    extern const char*   pingPong_png;
    const int            pingPong_pngSize = 6994;

    extern const char*   position_png;
    const int            position_pngSize = 7156;

    extern const char*   model_png;
    const int            model_pngSize = 3682;

    extern const char*   script_png;
    const int            script_pngSize = 2116;

    extern const char*   timeline_png;
    const int            timeline_pngSize = 1253;

    extern const char*   default_cdlayout;
    const int            default_cdlayoutSize = 3222;

    extern const char*   fadeIn_png;
    const int            fadeIn_pngSize = 1868;

    extern const char*   fadeOut_png;
    const int            fadeOut_pngSize = 392;

    extern const char*   icon_png;
    const int            icon_pngSize = 243986;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 18;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
