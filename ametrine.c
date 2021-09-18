//----------------------------------------------------------------------------//
//                                  INCLUDES                                  //
//----------------------------------------------------------------------------//

#include <GL/gl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#if (defined _WIN32 || defined _WIN64)
    #define OEMRESOURCE
    #define AM_WINDOWS
    #include <windows.h>
#else
    #define AM_LINUX
    #include <X11/Xatom.h>
    #include <X11/Xlib.h>
    #include <GL/glx.h>
    #include <sys/stat.h>
#endif

//----------------------------------------------------------------------------//
//                                END INCLUDES                                //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                               TYPES AND DEFS                               //
//----------------------------------------------------------------------------//

#define am_malloc(size) malloc(size)
#define am_free(mem) free((void*)mem)
#define am_realloc(mem, size) realloc(mem, size)
#define am_calloc(mem, size) calloc(mem, size)

typedef signed char am_int8;
typedef unsigned char am_uint8;
typedef signed short am_int16;
typedef unsigned short am_uint16;
typedef signed int  am_int32;
typedef unsigned int  am_uint32;
typedef signed long long int am_int64; //Can be 4 bytes on 32bit OSes
typedef unsigned long long int am_uint64;
typedef float am_float32;
typedef double am_float64;
typedef enum {false, true} am_bool;
typedef enum {FAILURE, SUCCESS, IN_PROGRESS} am_result;

#define AM_MAX_KEYCODE_COUNT 512
#define AM_ROOT_WIN_CLASS "AM_ROOT"
#define AM_CHILD_WIN_CLASS "AM_CHILD"

#if defined(AM_LINUX)
    #define _CALL *
    #define amgl_get_proc_address(str) glXGetProcAddress((unsigned char*)(str))
#else
    //TODO: Windows counterparts
#endif

//HACK: Temporary
am_bool temp_check = true;

//REVIEW: OpenGL functions


typedef void (_CALL PFNGLXSWAPINTERVALEXTPROC) (Display *dpy, GLXDrawable drawable, int interval);
typedef GLuint (_CALL PFNGLCREATESHADERPROC) (GLenum type);
typedef void (_CALL PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (_CALL PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef void (_CALL PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint *params);
typedef void (_CALL PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (_CALL PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (_CALL PFNGLDELETESHADERPROC) (GLuint shader);
typedef GLuint (_CALL PFNGLCREATEPROGRAMPROC) (void);
typedef void (_CALL PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (_CALL PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (_CALL PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint *params);
typedef void (_CALL PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (_CALL PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (_CALL PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (_CALL PFNGLGENBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void (_CALL PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint *buffers);
typedef void (_CALL PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (_CALL PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (_CALL PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
typedef void (_CALL PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint *arrays);
typedef void (_CALL PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (_CALL PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void (_CALL PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef GLint (_CALL PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (_CALL PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef GLint (_CALL PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar *name);
typedef void (_CALL PFNGLGENERATEMIPMAPPROC) (GLenum target);

PFNGLXSWAPINTERVALEXTPROC glXSwapIntervalEXT = NULL;
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLDETACHSHADERPROC glDetachShader = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = NULL;


//----------------------------------------------------------------------------//
//                             END TYPES AND DEFS                             //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                               DYNAMIC  ARRAY                               //
//----------------------------------------------------------------------------//

typedef struct am_dyn_array {
    am_uint8 *data;
    am_uint32 length;
    am_uint32 alloc_length;
    am_uint32 element_size;
} am_dyn_array;

void am_dyn_array_init(am_dyn_array *array, am_uint32 element_size);
am_bool am_dyn_array_resize(am_dyn_array *array, am_uint32 newsize);
am_bool am_dyn_array_push(am_dyn_array *array, void *data, am_uint32 length);
am_bool am_dyn_array_insert(am_dyn_array *array, void *data, am_uint32 length, am_uint32 index);
am_bool am_dyn_array_pop(am_dyn_array *array, am_uint32 length);
am_bool am_dyn_array_remove(am_dyn_array *array, am_uint32 index, am_uint32 length);
void am_dyn_array_cleanup(am_dyn_array *array);

#define am_dyn_array_create(name, type) am_dyn_array name; am_dyn_array_init(&name, (uint32)sizeof(type))
#define am_dyn_array_data_length(array, length) ((array)->element_size * (length))
#define am_dyn_array_data_pos(array, index) ((array)->data + am_dyn_array_data_length((array), (index)))
#define am_dyn_array_data_retrieve(array, type, index) (&(((type*) (void*) (array)->data)[(index)]))

//----------------------------------------------------------------------------//
//                             END DYNAMIC  ARRAY                             //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                                    MATH                                    //
//----------------------------------------------------------------------------//


typedef struct am_vec2 {
    union {
        am_float32 xy[2];
        struct {
            am_float32 x, y;
        };
    };
} am_vec2;

static inline am_vec2 am_vec2_add(am_vec2 vec0, am_vec2 vec1);
static inline am_vec2 am_vec2_sub(am_vec2 vec0, am_vec2 vec1);
static inline am_vec2 am_vec2_mul(am_vec2 vec0, am_vec2 vec1);
static inline am_vec2 am_vec2_div(am_vec2 vec0, am_vec2 vec1);
static inline am_vec2 am_vec2_scale(am_float32 scalar, am_vec2 vec);

//----------------------------------------------------------------------------//
//                                  END MATH                                  //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                                  PLATFORM                                  //
//----------------------------------------------------------------------------//


#if defined(AM_LINUX) 
    #define AM_WINDOW_ROOT_PARENT DefaultRootWindow(am_engine_get_subsystem(platform)->display) 
#else 
    #define AM_WINDOW_ROOT_PARENT 0
#endif

typedef enum am_key_map {
    AM_KEYCODE_INVALID,
    AM_KEYCODE_ESCAPE,
    AM_KEYCODE_F1,
    AM_KEYCODE_F2,
    AM_KEYCODE_F3,
    AM_KEYCODE_F4,
    AM_KEYCODE_F5,
    AM_KEYCODE_F6,
    AM_KEYCODE_F7,
    AM_KEYCODE_F8,
    AM_KEYCODE_F9,
    AM_KEYCODE_F10,
    AM_KEYCODE_F11,
    AM_KEYCODE_F12,
    AM_KEYCODE_F13,
    AM_KEYCODE_F14,
    AM_KEYCODE_F15,
    AM_KEYCODE_F16,
    AM_KEYCODE_F17,
    AM_KEYCODE_F18,
    AM_KEYCODE_F19,
    AM_KEYCODE_F20,
    AM_KEYCODE_F21,
    AM_KEYCODE_F22,
    AM_KEYCODE_F23,
    AM_KEYCODE_F24,
    AM_KEYCODE_F25,
    AM_KEYCODE_PRINT_SCREEN,
    AM_KEYCODE_SCROLL_LOCK,
    AM_KEYCODE_PAUSE,
    AM_KEYCODE_ACCENT_GRAVE,
    AM_KEYCODE_1,
    AM_KEYCODE_2,
    AM_KEYCODE_3,
    AM_KEYCODE_4,
    AM_KEYCODE_5,
    AM_KEYCODE_6,
    AM_KEYCODE_7,
    AM_KEYCODE_8,
    AM_KEYCODE_9,
    AM_KEYCODE_0,
    AM_KEYCODE_MINUS,
    AM_KEYCODE_EQUAL,
    AM_KEYCODE_BACKSPACE,
    AM_KEYCODE_INSERT,
    AM_KEYCODE_HOME,
    AM_KEYCODE_PAGE_UP,
    AM_KEYCODE_NUMPAD_NUM,
    AM_KEYCODE_NUMPAD_DIVIDE,
    AM_KEYCODE_NUMPAD_MULTIPLY,
    AM_KEYCODE_NUMPAD_SUBTRACT,
    AM_KEYCODE_TAB,
    AM_KEYCODE_Q,
    AM_KEYCODE_W,
    AM_KEYCODE_E,
    AM_KEYCODE_R,
    AM_KEYCODE_T,
    AM_KEYCODE_Y,
    AM_KEYCODE_U,
    AM_KEYCODE_I,
    AM_KEYCODE_O,
    AM_KEYCODE_P,
    AM_KEYCODE_LEFT_SQUARE_BRACKET,
    AM_KEYCODE_RIGHT_SQUARE_BRACKET,
    AM_KEYCODE_BACKSLASH,
    AM_KEYCODE_DELETE,
    AM_KEYCODE_END,
    AM_KEYCODE_PAGE_DOWN,
    AM_KEYCODE_NUMPAD_7,
    AM_KEYCODE_NUMPAD_8,
    AM_KEYCODE_NUMPAD_9,
    AM_KEYCODE_CAPS_LOCK,
    AM_KEYCODE_A,
    AM_KEYCODE_S,
    AM_KEYCODE_D,
    AM_KEYCODE_F,
    AM_KEYCODE_G,
    AM_KEYCODE_H,
    AM_KEYCODE_J,
    AM_KEYCODE_K,
    AM_KEYCODE_L,
    AM_KEYCODE_SEMICOLON,
    AM_KEYCODE_APOSTROPHE,
    AM_KEYCODE_ENTER,
    AM_KEYCODE_NUMPAD_4,
    AM_KEYCODE_NUMPAD_5,
    AM_KEYCODE_NUMPAD_6,
    AM_KEYCODE_NUMPAD_ADD,
    AM_KEYCODE_LEFT_SHIFT,
    AM_KEYCODE_Z,
    AM_KEYCODE_X,
    AM_KEYCODE_C,
    AM_KEYCODE_V,
    AM_KEYCODE_B,
    AM_KEYCODE_N,
    AM_KEYCODE_M,
    AM_KEYCODE_COMMA,
    AM_KEYCODE_PERIOD,
    AM_KEYCODE_SLASH,
    AM_KEYCODE_RIGHT_SHIFT,
    AM_KEYCODE_UP_ARROW,
    AM_KEYCODE_NUMPAD_1,
    AM_KEYCODE_NUMPAD_2,
    AM_KEYCODE_NUMPAD_3,
    AM_KEYCODE_LEFT_CONTROL,
    AM_KEYCODE_LEFT_META,
    AM_KEYCODE_LEFT_ALT,
    AM_KEYCODE_SPACE, 
    AM_KEYCODE_RIGHT_ALT,
    AM_KEYCODE_RIGHT_META,
    AM_KEYCODE_MENU,
    AM_KEYCODE_RIGHT_CONTROL,
    AM_KEYCODE_LEFT_ARROW,
    AM_KEYCODE_DOWN_ARROW,
    AM_KEYCODE_RIGHT_ARROW, 
    AM_KEYCODE_NUMPAD_0,
    AM_KEYCODE_NUMPAD_DECIMAL,
    AM_KEYCODE_NUMPAD_EQUAL,
    AM_KEYCODE_NUMPAD_ENTER,
    AM_KEYCODE_COUNT
} am_key_map;

typedef enum am_mouse_map {
    AM_MOUSE_BUTTON_INVALID,
    AM_MOUSE_BUTTON_LEFT,
    AM_MOUSE_BUTTON_RIGHT,
    AM_MOUSE_BUTTON_MIDDLE,
    AM_MOUSE_BUTTON_COUNT
} am_mouse_map;

typedef enum am_events {
    AM_EVENT_INVALID,
    AM_EVENT_KEY_PRESS,
    AM_EVENT_KEY_RELEASE,
    AM_EVENT_MOUSE_MOTION,
    AM_EVENT_MOUSE_BUTTON_PRESS,
    AM_EVENT_MOUSE_BUTTON_RELEASE,
    AM_EVENT_MOUSE_SCROLL_UP,
    AM_EVENT_MOUSE_SCROLL_DOWN,
    AM_EVENT_WINDOW_SIZE,
    AM_EVENT_WINDOW_MOTION,
    AM_EVENT_COUNT
} am_events;

//TODO: Remove window_ prefix, it is clear the data belongs to a window 
typedef struct am_window_info {
    am_uint64 parent;
    const char* title;
    am_uint32 width;
    am_uint32 height; 
    am_uint32 x;
    am_uint32 y;
    //am_vec2i window_position; 
    am_bool is_fullscreen; //Useless for child windows
} am_window_info;

typedef struct am_window {
    am_uint64 handle;
    am_int32 am_id;
    am_window_info info;
    am_window_info cache;
    
    #if defined(AM_LINUX)
        Colormap colormap;
        XVisualInfo *visual_info;
        GLXContext context;
    #else
        //Context windows
    #endif
} am_window;

typedef struct am_platform_callbacks {
    void (*am_platform_key_callback)(am_uint64, am_key_map, am_events);
    void (*am_platform_mouse_button_callback)(am_uint64, am_mouse_map, am_events);
    void (*am_platform_mouse_motion_callback)(am_uint64, am_int32, am_int32, am_events);
    void (*am_platform_mouse_scroll_callback)(am_uint64, am_events);
    void (*am_platform_window_size_callback)(am_uint64, am_uint32, am_uint32, am_events);
    void (*am_platform_window_motion_callback)(am_uint64, am_uint32, am_uint32, am_events);
} am_platform_callbacks;

typedef struct am_platform_input {
    am_key_map keycodes[AM_MAX_KEYCODE_COUNT]; //LUT
    am_bool keyboard_map[AM_KEYCODE_COUNT]; 
    am_bool prev_keyboard_map[AM_KEYCODE_COUNT];
    am_bool mouse_map[AM_MOUSE_BUTTON_COUNT];
    am_bool prev_mouse_map[AM_MOUSE_BUTTON_COUNT];
    am_int32 wheel_delta;
    am_uint32 mouse_x;
    am_uint32 mouse_y;
    am_bool mouse_moved;
} am_platform_input;

typedef struct am_platform_time {
    am_uint64 offset;
    am_uint64 frequency;
} am_platform_time;

typedef struct am_platform {
    #if defined(AM_LINUX)
        Display *display;
    #endif
    am_dyn_array windows; //of am_window
    am_platform_input input;
    am_platform_time time;
    am_platform_callbacks callbacks;
} am_platform;


//Platform 
#if defined(AM_LINUX)
am_key_map am_platform_translate_keysym(KeySym *key_syms, am_int32 width);
#endif
am_mouse_map am_platform_translate_button(am_uint32 button);
am_platform *am_platform_create();
void am_platform_poll_events();
#if defined(AM_WINDOWS) 
LRESULT CALLBACK am_platform_win32_event_handler(HWND handle, am_uint32 event, WPARAM wparam, LPARAM lparam);
#else
void am_platform_linux_event_handler(XEvent *xevent);
#endif
void am_platform_update(am_platform *platform);
void am_platform_terminate(am_platform *platform);

//Keyboard input
void am_platform_key_press(am_key_map key); 
void am_platform_key_release(am_key_map key); 
am_bool am_platform_key_pressed(am_key_map key); 
am_bool am_platform_key_down(am_key_map key);
am_bool am_platform_key_released(am_key_map key); 
am_bool am_platform_key_up(am_key_map key);

//Mouse input
void am_platform_mouse_button_press(am_mouse_map button); 
void am_platform_mouse_button_release(am_mouse_map button); 
am_bool am_platform_mouse_button_pressed(am_mouse_map button); 
am_bool am_platform_mouse_button_down(am_mouse_map button); 
am_bool am_platform_mouse_button_released(am_mouse_map button); 
am_bool am_platform_mouse_button_up(am_mouse_map button);
void am_platform_mouse_position(am_int32 *x, am_int32 *y); 
am_vec2 am_platform_mouse_positionv();
am_int32 am_platform_mouse_wheel_delta();
am_bool am_platform_mouse_moved();

//Platform default callbacks
void am_platform_key_callback_default(am_uint64 window_handle, am_key_map key, am_events event);
void am_platform_mouse_button_callback_default(am_uint64 window_handle, am_mouse_map button, am_events event);
void am_platform_mouse_motion_callback_default(am_uint64 window_handle, am_int32 x, am_int32 y, am_events event);
void am_platform_mouse_scroll_callback_default(am_uint64 window_handle, am_events event);
void am_platform_window_size_callback_default(am_uint64 window_handle, am_uint32 width, am_uint32 height, am_events event);
void am_platform_window_motion_callback_default(am_uint64 window_handle, am_uint32 x, am_uint32 y, am_events event);

#define am_platform_set_key_callback(platform, callback) platform->callbacks.am_platform_key_callback = callback
#define am_platform_set_mouse_button_callback(platform, callback) platform->callbacks.am_platform_mouse_button_callback = callback
#define am_platform_set_mouse_motion_callback(platform, callback) platform->callbacks.am_platform_mouse_motion_callback = callback
#define am_platform_set_mouse_scroll_callback(platform, callback) platform->callbacks.am_platform_mouse_scroll_callback = callback  
#define am_platform_set_window_size_callback(platform, callback) platform->callbacks.am_platform_window_size_callback = callback  
#define am_platform_set_window_motion_callback(platform, callback) platform->callbacks.am_platform_window_motion_callback = callback  

//Windows
am_window *am_platform_window_create(am_window_info window_info);
am_window *am_platform_window_lookup(am_uint64 handle); 
am_window *am_platform_window_retrieve(am_uint32 index);
am_int32 am_platform_window_index_lookup(am_uint64 handle);
am_bool am_platform_window_exists(am_uint64 handle);
void am_platform_window_resize(am_uint64 handle, am_uint32 width, am_uint32 height);
void am_platform_window_move(am_uint64 handle, am_uint32 x, am_uint32 y);
void am_platform_window_fullscreen(am_uint64 handle, am_bool state);
void am_platform_window_terminate(am_window *window);


//Time
void am_platform_timer_create();
void am_platform_timer_sleep(am_float32 ms);
am_uint64 am_platform_timer_value();
am_uint64 am_platform_elapsed_time();


//----------------------------------------------------------------------------//
//                                END PLATFORM                                //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                                  START GL                                  //
//----------------------------------------------------------------------------//

//https://www.khronos.org/opengl/wiki/OpenGL_Object
//BUFFERS shaders, textures, vertex, index, frame, uniform
//TODO: Windows side of things


//TODO: Shaders
//Shader & program
//Removed compute shader for now
typedef enum amgl_shader_type {
    AMGL_SHADER_VERTEX,
    AMGL_SHADER_FRAGMENT
} amgl_shader_type;

typedef struct amgl_shader_source_info {
    amgl_shader_type type;
    char *source;
} amgl_shader_source_info;

typedef struct amgl_shader_info {
    //char *identifier;
    amgl_shader_source_info *sources;
    am_uint32 num;
} amgl_shader_info;

//TODO: Figure out if it should be amgl_shader or amgl_shader_program
typedef struct amgl_shader {
    am_uint32 handle;
    am_int32 am_id;
    amgl_shader_info info;
} amgl_shader;

//TODO: Textures
//Textures
typedef struct amgl_texture_info {
    char *identifier;
    void *data;
    am_uint32 width;
    am_uint32 height;
    GLenum format;
    GLenum min_filter;
    GLenum mag_filter;
    GLenum mip_filter;
    am_uint32 mip_num;
    GLenum wrap_s;
    GLenum wrap_t;
} amgl_texture_info;

typedef struct amgl_texture {
    am_uint32 handle;
    am_int32 am_id;
    am_bool render_traget;
    amgl_texture_info info;
} amgl_texture;

//Shaders
amgl_shader *amgl_shader_create(amgl_shader_info info);
void amgl_shader_source_load_from_file(const char *path, amgl_shader_source_info *info);
void amgl_shader_source_load_from_memory(const void *memory, amgl_shader_source_info *info, size_t size);
void amgl_shader_destroy(amgl_shader *shader);

//Textures
amgl_texture *amgl_texture_create(amgl_texture_info info);
void amgl_texture_load_from_file(const char *path, amgl_texture_info *info, am_bool flip);
void amgl_texture_load_from_memory(const void *memory, amgl_texture_info *info, size_t size, am_bool flip);
//TODO: Implement | void amgl_texture_update(amgl_texture texture);
void amgl_texture_destroy(amgl_texture *texture);

//TODO: These
void amgl_init(); //Create arrays for shaders, vertex b, index b, frame b etc, init gl addresses
void amgl_terminate();
void amgl_set_viewport(am_int32 x, am_int32 y, am_int32 width, am_int32 height);
void amgl_vsync(am_window *window, am_bool state);

//----------------------------------------------------------------------------//
//                                   END GL                                   //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                                   ENGINE                                   //
//----------------------------------------------------------------------------//

typedef struct am_engine_info {
    void (*init)();
    void (*update)();
    void (*shutdown)();
    am_bool is_running;
    //REVIEW: Vsync global for all windows for now
    am_bool vsync_enabled;
    //Variables marked with initial_ will not be updated during runtime
    char *initial_title;
    am_bool initial_fullscreen;
    am_uint32 initial_width;
    am_uint32 initial_height;
    am_uint32 initial_x;
    am_uint32 initial_y;
} am_engine_info;

//REVIEW: 
typedef struct amgl_ctx_data {
    am_dyn_array textures; //of amgl_texture
    am_dyn_array shaders; //of amgl_shader
    //TODO: Rest of object arrays and other user data
} amgl_ctx_data;

typedef struct am_engine {
    am_engine_info info;
    am_platform *platform;
    amgl_ctx_data ctx_data;
    //am_audio audio; TODO: Implement
    //am_pfngl pfngl; TODO: Implement
} am_engine;

//The only one that should exist
am_engine *_am_engine_instance;

//TODO: Implement engine part
#define am_engine_get_instance() _am_engine_instance
#define am_engine_get_subsystem(sys) am_engine_get_instance()->sys

void am_engine_create(am_engine_info engine_info);
void am_engine_terminate();


//----------------------------------------------------------------------------//
//                                 END ENGINE                                 //
//----------------------------------------------------------------------------//

/*

ENGINE
   |
   - PLATFORM
   |    |
   |    - INPUT
   |    - TIME
   |    - WINDOW ARRAY
   |    - CALLBACKS
   |
   - AUDIO
   |
   - PFNGL* //Maybe not needed here
   |   |
   |   - Pointers to all OpenGL functions needed
   |
   - USER DATA POINTER
*/

//----------------------------------------------------------------------------//
//                             DYNAMIC  ARRAY IMPL                            //
//----------------------------------------------------------------------------//

void am_dyn_array_init(am_dyn_array *array, am_uint32 element_size) {
    array->data = NULL;
    array->length = 0;
    array->alloc_length = 0;
    array->element_size = element_size;
};

am_bool am_dyn_array_resize(am_dyn_array *array, am_uint32 new_length) {
    if (new_length > array->alloc_length) {
        void *buffer = NULL;
        buffer = realloc(array->data, (new_length + (array->alloc_length >> 1u))*am_dyn_array_data_length(array, 1));
        if (buffer) {
            array->alloc_length = new_length + (array->alloc_length >> 1u);
            array->data = (am_uint8*) buffer;
        } else return false;
    };
    return true;
};

am_bool am_dyn_array_push(am_dyn_array *array, void *data, am_uint32 length) {
    if (length >=0) { //Useless check, just keeping the same function type for prettiness
        am_dyn_array_resize(array, array->length + length);
        memcpy(am_dyn_array_data_pos(array, array->length), data, am_dyn_array_data_length(array, length));
        array->length += length;
        return true;
    };
    return false;
};

am_bool am_dyn_array_insert(am_dyn_array *array, void *data, am_uint32 length, am_uint32 index) {
    if (index <= array->length) {
        am_dyn_array_resize(array, array->length + length);
        memcpy(am_dyn_array_data_pos(array, index+length), am_dyn_array_data_pos(array, index), am_dyn_array_data_length(array, length));
        memcpy(am_dyn_array_data_pos(array, index), data, am_dyn_array_data_length(array, length));
        array->length += length;
        return true;
    };
    return false;
};

am_bool am_dyn_array_pop(am_dyn_array *array, am_uint32 length) {
    if (length <= array->length) {
        array->length -= length;
        return true;
    };
    return false;
};

am_bool am_dyn_array_remove(am_dyn_array *array, am_uint32 index, am_uint32 length) {
    if (index + length > array->length) return false;
    memcpy(am_dyn_array_data_pos(array, index), am_dyn_array_data_pos(array, index+length), am_dyn_array_data_length(array, array->length - length));
    array->length -= length;
    return true;
};

void am_dyn_array_cleanup(am_dyn_array *array) {
    array->length = 0;
    array->element_size = 0;
    array->alloc_length = 0;
    free((void*)array->data);
    array->data = NULL;
};

//----------------------------------------------------------------------------//
//                           END DYNAMIC  ARRAY IMPL                          //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                                  MATH IMPL                                 //
//----------------------------------------------------------------------------//

static inline am_vec2 am_vec2_add(am_vec2 vec0, am_vec2 vec1) {
    am_vec2 v;
    v.x = vec0.x + vec1.x;
    v.y = vec0.y + vec1.y;
    return v;
};

static inline am_vec2 am_vec2_sub(am_vec2 vec0, am_vec2 vec1) {
    am_vec2 v;
    v.x = vec0.x - vec1.x;
    v.y = vec0.y - vec1.y;
    return v;
};

static inline am_vec2 am_vec2_mul(am_vec2 vec0, am_vec2 vec1) {
    am_vec2 v;
    v.x = vec0.x * vec1.x;
    v.y = vec0.y * vec1.y;
    return v;
};

static inline am_vec2 am_vec2_div(am_vec2 vec0, am_vec2 vec1){
    am_vec2 v;
    v.x = vec0.x / vec1.x;
    v.y = vec0.y / vec1.y;
    return v;
};

static inline am_vec2 am_vec2_scale(am_float32 scalar, am_vec2 vec){
    am_vec2 v;
    v.x = scalar * vec.x;
    v.y = scalar * vec.y;
    return v;
};

//----------------------------------------------------------------------------//
//                               END MATH IMPL                                //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                                PLATFORM IMPL                               //
//----------------------------------------------------------------------------//


#if defined(AM_LINUX)
am_key_map am_platform_translate_keysym(KeySym *key_syms, am_int32 width) {
    if (width > 1) {
        switch (key_syms[1]) {
            case XK_KP_0:           return AM_KEYCODE_NUMPAD_0;
            case XK_KP_1:           return AM_KEYCODE_NUMPAD_1;
            case XK_KP_2:           return AM_KEYCODE_NUMPAD_2;
            case XK_KP_3:           return AM_KEYCODE_NUMPAD_3;
            case XK_KP_4:           return AM_KEYCODE_NUMPAD_4;
            case XK_KP_5:           return AM_KEYCODE_NUMPAD_5;
            case XK_KP_6:           return AM_KEYCODE_NUMPAD_6;
            case XK_KP_7:           return AM_KEYCODE_NUMPAD_7;
            case XK_KP_8:           return AM_KEYCODE_NUMPAD_8;
            case XK_KP_9:           return AM_KEYCODE_NUMPAD_9;
            case XK_KP_Separator:
            case XK_KP_Decimal:     return AM_KEYCODE_NUMPAD_DECIMAL;
            case XK_KP_Equal:       return AM_KEYCODE_NUMPAD_EQUAL;
            case XK_KP_Enter:       return AM_KEYCODE_NUMPAD_ENTER;
            default:                break;
        };
    };

    switch (key_syms[0]) {
        case XK_Escape:         return AM_KEYCODE_ESCAPE;
        case XK_Tab:            return AM_KEYCODE_TAB;
        case XK_Shift_L:        return AM_KEYCODE_LEFT_SHIFT;
        case XK_Shift_R:        return AM_KEYCODE_RIGHT_SHIFT;
        case XK_Control_L:      return AM_KEYCODE_LEFT_CONTROL;
        case XK_Control_R:      return AM_KEYCODE_RIGHT_CONTROL;
        case XK_Alt_L:          return AM_KEYCODE_LEFT_ALT;
        case XK_Alt_R:          return AM_KEYCODE_RIGHT_ALT;
        case XK_Super_L:        return AM_KEYCODE_LEFT_META;
        case XK_Super_R:        return AM_KEYCODE_RIGHT_META;
        case XK_Menu:           return AM_KEYCODE_MENU;
        case XK_Num_Lock:       return AM_KEYCODE_NUMPAD_NUM;
        case XK_Caps_Lock:      return AM_KEYCODE_CAPS_LOCK;
        case XK_Print:          return AM_KEYCODE_PRINT_SCREEN;
        case XK_Scroll_Lock:    return AM_KEYCODE_SCROLL_LOCK;
        case XK_Pause:          return AM_KEYCODE_PAUSE;
        case XK_Delete:         return AM_KEYCODE_DELETE;
        case XK_BackSpace:      return AM_KEYCODE_BACKSPACE;
        case XK_Return:         return AM_KEYCODE_ENTER;
        case XK_Home:           return AM_KEYCODE_HOME;
        case XK_End:            return AM_KEYCODE_END;
        case XK_Page_Up:        return AM_KEYCODE_PAGE_UP;
        case XK_Page_Down:      return AM_KEYCODE_PAGE_DOWN;
        case XK_Insert:         return AM_KEYCODE_INSERT;
        case XK_Left:           return AM_KEYCODE_LEFT_ARROW;
        case XK_Right:          return AM_KEYCODE_RIGHT_ARROW;
        case XK_Down:           return AM_KEYCODE_DOWN_ARROW;
        case XK_Up:             return AM_KEYCODE_UP_ARROW;
        case XK_F1:             return AM_KEYCODE_F1;
        case XK_F2:             return AM_KEYCODE_F2;
        case XK_F3:             return AM_KEYCODE_F3;
        case XK_F4:             return AM_KEYCODE_F4;
        case XK_F5:             return AM_KEYCODE_F5;
        case XK_F6:             return AM_KEYCODE_F6;
        case XK_F7:             return AM_KEYCODE_F7;
        case XK_F8:             return AM_KEYCODE_F8;
        case XK_F9:             return AM_KEYCODE_F9;
        case XK_F10:            return AM_KEYCODE_F10;
        case XK_F11:            return AM_KEYCODE_F11;
        case XK_F12:            return AM_KEYCODE_F12;
        case XK_F13:            return AM_KEYCODE_F13;
        case XK_F14:            return AM_KEYCODE_F14;
        case XK_F15:            return AM_KEYCODE_F15;
        case XK_F16:            return AM_KEYCODE_F16;
        case XK_F17:            return AM_KEYCODE_F17;
        case XK_F18:            return AM_KEYCODE_F18;
        case XK_F19:            return AM_KEYCODE_F19;
        case XK_F20:            return AM_KEYCODE_F20;
        case XK_F21:            return AM_KEYCODE_F21;
        case XK_F22:            return AM_KEYCODE_F22;
        case XK_F23:            return AM_KEYCODE_F23;
        case XK_F24:            return AM_KEYCODE_F24;
        case XK_F25:            return AM_KEYCODE_F25;
        case XK_KP_Divide:      return AM_KEYCODE_NUMPAD_DIVIDE;
        case XK_KP_Multiply:    return AM_KEYCODE_NUMPAD_MULTIPLY;
        case XK_KP_Subtract:    return AM_KEYCODE_NUMPAD_SUBTRACT;
        case XK_KP_Add:         return AM_KEYCODE_NUMPAD_ADD;
        case XK_KP_Insert:      return AM_KEYCODE_NUMPAD_0;
        case XK_KP_End:         return AM_KEYCODE_NUMPAD_1;
        case XK_KP_Down:        return AM_KEYCODE_NUMPAD_2;
        case XK_KP_Page_Down:   return AM_KEYCODE_NUMPAD_3;
        case XK_KP_Left:        return AM_KEYCODE_NUMPAD_4;
        case XK_KP_Right:       return AM_KEYCODE_NUMPAD_6;
        case XK_KP_Home:        return AM_KEYCODE_NUMPAD_7;
        case XK_KP_Up:          return AM_KEYCODE_NUMPAD_8;
        case XK_KP_Page_Up:     return AM_KEYCODE_NUMPAD_9;
        case XK_KP_Delete:      return AM_KEYCODE_NUMPAD_DECIMAL;
        case XK_KP_Equal:       return AM_KEYCODE_NUMPAD_EQUAL;
        case XK_KP_Enter:       return AM_KEYCODE_NUMPAD_ENTER;
        case XK_a:              return AM_KEYCODE_A;
        case XK_b:              return AM_KEYCODE_B;
        case XK_c:              return AM_KEYCODE_C;
        case XK_d:              return AM_KEYCODE_D;
        case XK_e:              return AM_KEYCODE_E;
        case XK_f:              return AM_KEYCODE_F;
        case XK_g:              return AM_KEYCODE_G;
        case XK_h:              return AM_KEYCODE_H;
        case XK_i:              return AM_KEYCODE_I;
        case XK_j:              return AM_KEYCODE_J;
        case XK_k:              return AM_KEYCODE_K;
        case XK_l:              return AM_KEYCODE_L;
        case XK_m:              return AM_KEYCODE_M;
        case XK_n:              return AM_KEYCODE_N;
        case XK_o:              return AM_KEYCODE_O;
        case XK_p:              return AM_KEYCODE_P;
        case XK_q:              return AM_KEYCODE_Q;
        case XK_r:              return AM_KEYCODE_R;
        case XK_s:              return AM_KEYCODE_S;
        case XK_t:              return AM_KEYCODE_T;
        case XK_u:              return AM_KEYCODE_U;
        case XK_v:              return AM_KEYCODE_V;
        case XK_w:              return AM_KEYCODE_W;
        case XK_x:              return AM_KEYCODE_X;
        case XK_y:              return AM_KEYCODE_Y;
        case XK_z:              return AM_KEYCODE_Z;
        case XK_1:              return AM_KEYCODE_1;
        case XK_2:              return AM_KEYCODE_2;
        case XK_3:              return AM_KEYCODE_3;
        case XK_4:              return AM_KEYCODE_4;
        case XK_5:              return AM_KEYCODE_5;
        case XK_6:              return AM_KEYCODE_6;
        case XK_7:              return AM_KEYCODE_7;
        case XK_8:              return AM_KEYCODE_8;
        case XK_9:              return AM_KEYCODE_9;
        case XK_0:              return AM_KEYCODE_0;
        case XK_space:          return AM_KEYCODE_SPACE;
        case XK_minus:          return AM_KEYCODE_MINUS;
        case XK_equal:          return AM_KEYCODE_EQUAL;
        case XK_bracketleft:    return AM_KEYCODE_LEFT_SQUARE_BRACKET;
        case XK_bracketright:   return AM_KEYCODE_RIGHT_SQUARE_BRACKET;
        case XK_backslash:      return AM_KEYCODE_BACKSLASH;
        case XK_semicolon:      return AM_KEYCODE_SEMICOLON;
        case XK_apostrophe:     return AM_KEYCODE_APOSTROPHE;
        case XK_grave:          return AM_KEYCODE_ACCENT_GRAVE;
        case XK_comma:          return AM_KEYCODE_COMMA;
        case XK_period:         return AM_KEYCODE_PERIOD;
        case XK_slash:          return AM_KEYCODE_SLASH;
        default:                return AM_KEYCODE_INVALID;
    };
};
#endif

am_mouse_map am_platform_translate_button(am_uint32 button) {
    switch (button) {
        case 1: return AM_MOUSE_BUTTON_LEFT;
        case 2: return AM_MOUSE_BUTTON_MIDDLE;
        case 3: return AM_MOUSE_BUTTON_RIGHT;
    };
    return AM_MOUSE_BUTTON_INVALID;
};

am_platform *am_platform_create() {
    am_platform *platform = (am_platform*)am_malloc(sizeof(am_platform));
    if (platform == NULL) printf("[FAIL] Could not allocate memory for platform!\n");
    assert(platform != NULL);
    am_dyn_array_init(&platform->windows, sizeof(am_window));
    
    #if defined(AM_LINUX)
    platform->display = XOpenDisplay(NULL);
    memset(platform->input.keycodes, -1, sizeof(platform->input.keycodes));
    am_int32 min, max;
    XDisplayKeycodes(platform->display, &min, &max);
    am_int32 width;
    KeySym *key_syms = XGetKeyboardMapping(platform->display, min, max - min + 1, &width);
    for (am_int32 i = min; i < max; i++) platform->input.keycodes[i] = am_platform_translate_keysym(&key_syms[(i-min)*width], width);
    XFree(key_syms);
    #else
    platform->input.keycodes[0x00B] = AM_KEYCODE_0;
    platform->input.keycodes[0x002] = AM_KEYCODE_1;
    platform->input.keycodes[0x003] = AM_KEYCODE_2;
    platform->input.keycodes[0x004] = AM_KEYCODE_3;
    platform->input.keycodes[0x005] = AM_KEYCODE_4;
    platform->input.keycodes[0x006] = AM_KEYCODE_5;
    platform->input.keycodes[0x007] = AM_KEYCODE_6;
    platform->input.keycodes[0x008] = AM_KEYCODE_7;
    platform->input.keycodes[0x009] = AM_KEYCODE_8;
    platform->input.keycodes[0x00A] = AM_KEYCODE_9;
    platform->input.keycodes[0x01E] = AM_KEYCODE_A;
    platform->input.keycodes[0x030] = AM_KEYCODE_B;
    platform->input.keycodes[0x02E] = AM_KEYCODE_C;
    platform->input.keycodes[0x020] = AM_KEYCODE_D;
    platform->input.keycodes[0x012] = AM_KEYCODE_E;
    platform->input.keycodes[0x021] = AM_KEYCODE_F;
    platform->input.keycodes[0x022] = AM_KEYCODE_G;
    platform->input.keycodes[0x023] = AM_KEYCODE_H;
    platform->input.keycodes[0x017] = AM_KEYCODE_I;
    platform->input.keycodes[0x024] = AM_KEYCODE_J;
    platform->input.keycodes[0x025] = AM_KEYCODE_K;
    platform->input.keycodes[0x026] = AM_KEYCODE_L;
    platform->input.keycodes[0x032] = AM_KEYCODE_M;
    platform->input.keycodes[0x031] = AM_KEYCODE_N;
    platform->input.keycodes[0x018] = AM_KEYCODE_O;
    platform->input.keycodes[0x019] = AM_KEYCODE_P;
    platform->input.keycodes[0x010] = AM_KEYCODE_Q;
    platform->input.keycodes[0x013] = AM_KEYCODE_R;
    platform->input.keycodes[0x01F] = AM_KEYCODE_S;
    platform->input.keycodes[0x014] = AM_KEYCODE_T;
    platform->input.keycodes[0x016] = AM_KEYCODE_U;
    platform->input.keycodes[0x02F] = AM_KEYCODE_V;
    platform->input.keycodes[0x011] = AM_KEYCODE_W;
    platform->input.keycodes[0x02D] = AM_KEYCODE_X;
    platform->input.keycodes[0x015] = AM_KEYCODE_Y;
    platform->input.keycodes[0x02C] = AM_KEYCODE_Z;
    platform->input.keycodes[0x028] = AM_KEYCODE_APOSTROPHE;
    platform->input.keycodes[0x02B] = AM_KEYCODE_BACKSLASH;
    platform->input.keycodes[0x033] = AM_KEYCODE_COMMA;
    platform->input.keycodes[0x00D] = AM_KEYCODE_EQUAL;
    platform->input.keycodes[0x029] = AM_KEYCODE_ACCENT_GRAVE;
    platform->input.keycodes[0x01A] = AM_KEYCODE_LEFT_SQUARE_BRACKET;
    platform->input.keycodes[0x00C] = AM_KEYCODE_MINUS;
    platform->input.keycodes[0x034] = AM_KEYCODE_PERIOD;
    platform->input.keycodes[0x01B] = AM_KEYCODE_RIGHT_SQUARE_BRACKET;
    platform->input.keycodes[0x027] = AM_KEYCODE_SEMICOLON;
    platform->input.keycodes[0x035] = AM_KEYCODE_SLASH;
    platform->input.keycodes[0x00E] = AM_KEYCODE_BACKSPACE;
    platform->input.keycodes[0x153] = AM_KEYCODE_DELETE;
    platform->input.keycodes[0x14F] = AM_KEYCODE_END;
    platform->input.keycodes[0x01C] = AM_KEYCODE_ENTER;
    platform->input.keycodes[0x001] = AM_KEYCODE_ESCAPE;
    platform->input.keycodes[0x147] = AM_KEYCODE_HOME;
    platform->input.keycodes[0x152] = AM_KEYCODE_INSERT;
    platform->input.keycodes[0x15D] = AM_KEYCODE_MENU;
    platform->input.keycodes[0x151] = AM_KEYCODE_PAGE_DOWN;
    platform->input.keycodes[0x149] = AM_KEYCODE_PAGE_UP;
    platform->input.keycodes[0x045] = AM_KEYCODE_PAUSE;
    platform->input.keycodes[0x146] = AM_KEYCODE_PAUSE;
    platform->input.keycodes[0x039] = AM_KEYCODE_SPACE;
    platform->input.keycodes[0x00F] = AM_KEYCODE_TAB;
    platform->input.keycodes[0x03A] = AM_KEYCODE_CAPS_LOCK;
    platform->input.keycodes[0x145] = AM_KEYCODE_NUMPAD_NUM;
    platform->input.keycodes[0x046] = AM_KEYCODE_SCROLL_LOCK;
    platform->input.keycodes[0x03B] = AM_KEYCODE_F1;
    platform->input.keycodes[0x03C] = AM_KEYCODE_F2;
    platform->input.keycodes[0x03D] = AM_KEYCODE_F3;
    platform->input.keycodes[0x03E] = AM_KEYCODE_F4;
    platform->input.keycodes[0x03F] = AM_KEYCODE_F5;
    platform->input.keycodes[0x040] = AM_KEYCODE_F6;
    platform->input.keycodes[0x041] = AM_KEYCODE_F7;
    platform->input.keycodes[0x042] = AM_KEYCODE_F8;
    platform->input.keycodes[0x043] = AM_KEYCODE_F9;
    platform->input.keycodes[0x044] = AM_KEYCODE_F10;
    platform->input.keycodes[0x057] = AM_KEYCODE_F11;
    platform->input.keycodes[0x058] = AM_KEYCODE_F12;
    platform->input.keycodes[0x064] = AM_KEYCODE_F13;
    platform->input.keycodes[0x065] = AM_KEYCODE_F14;
    platform->input.keycodes[0x066] = AM_KEYCODE_F15;
    platform->input.keycodes[0x067] = AM_KEYCODE_F16;
    platform->input.keycodes[0x068] = AM_KEYCODE_F17;
    platform->input.keycodes[0x069] = AM_KEYCODE_F18;
    platform->input.keycodes[0x06A] = AM_KEYCODE_F19;
    platform->input.keycodes[0x06B] = AM_KEYCODE_F20;
    platform->input.keycodes[0x06C] = AM_KEYCODE_F21;
    platform->input.keycodes[0x06D] = AM_KEYCODE_F22;
    platform->input.keycodes[0x06E] = AM_KEYCODE_F23;
    platform->input.keycodes[0x076] = AM_KEYCODE_F24;
    platform->input.keycodes[0x038] = AM_KEYCODE_LEFT_ALT;
    platform->input.keycodes[0x01D] = AM_KEYCODE_LEFT_CONTROL;
    platform->input.keycodes[0x02A] = AM_KEYCODE_LEFT_SHIFT;
    platform->input.keycodes[0x15B] = AM_KEYCODE_LEFT_META;
    platform->input.keycodes[0x137] = AM_KEYCODE_PRINT_SCREEN;
    platform->input.keycodes[0x138] = AM_KEYCODE_RIGHT_ALT;
    platform->input.keycodes[0x11D] = AM_KEYCODE_RIGHT_CONTROL;
    platform->input.keycodes[0x036] = AM_KEYCODE_RIGHT_SHIFT;
    platform->input.keycodes[0x15C] = AM_KEYCODE_RIGHT_META;
    platform->input.keycodes[0x150] = AM_KEYCODE_DOWN_ARROW;
    platform->input.keycodes[0x14B] = AM_KEYCODE_LEFT_ARROW;
    platform->input.keycodes[0x14D] = AM_KEYCODE_RIGHT_ARROW;
    platform->input.keycodes[0x148] = AM_KEYCODE_UP_ARROW;
    platform->input.keycodes[0x052] = AM_KEYCODE_NUMPAD_0;
    platform->input.keycodes[0x04F] = AM_KEYCODE_NUMPAD_1;
    platform->input.keycodes[0x050] = AM_KEYCODE_NUMPAD_2;
    platform->input.keycodes[0x051] = AM_KEYCODE_NUMPAD_3;
    platform->input.keycodes[0x04B] = AM_KEYCODE_NUMPAD_4;
    platform->input.keycodes[0x04C] = AM_KEYCODE_NUMPAD_5;
    platform->input.keycodes[0x04D] = AM_KEYCODE_NUMPAD_6;
    platform->input.keycodes[0x047] = AM_KEYCODE_NUMPAD_7;
    platform->input.keycodes[0x048] = AM_KEYCODE_NUMPAD_8;
    platform->input.keycodes[0x049] = AM_KEYCODE_NUMPAD_9;
    platform->input.keycodes[0x04E] = AM_KEYCODE_NUMPAD_ADD;
    platform->input.keycodes[0x053] = AM_KEYCODE_NUMPAD_DECIMAL;
    platform->input.keycodes[0x135] = AM_KEYCODE_NUMPAD_DIVIDE;
    platform->input.keycodes[0x11C] = AM_KEYCODE_NUMPAD_ENTER;
    platform->input.keycodes[0x059] = AM_KEYCODE_NUMPAD_EQUAL;
    platform->input.keycodes[0x037] = AM_KEYCODE_NUMPAD_MULTIPLY;
    platform->input.keycodes[0x04A] = AM_KEYCODE_NUMPAD_SUBTRACT;

    WNDCLASS window_class = {0};
	window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    //REVIEW: Might not be necessary, come back after OpenGL is implemented
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.hInstance = GetModuleHandle(NULL);
	window_class.lpfnWndProc = am_platform_win32_event_handler;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.lpszMenuName = NULL;
	window_class.hbrBackground = NULL;
    //HACK: Colored here just for visuals
    window_class.hbrBackground = CreateSolidBrush(RGB(255, 0, 0));
	window_class.lpszClassName = "AM_ROOT";
	if (!RegisterClass(&window_class)) {
        printf("[FAIL] Failed to register root class!\n");
        return NULL;
    };
    //HACK: Colored here just for visuals
    window_class.hbrBackground = CreateSolidBrush(RGB(0, 0, 255));
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_PARENTDC;
    window_class.lpszClassName = "AM_CHILD";
    if (!RegisterClass(&window_class)) {
        printf("[FAIL] Failed to register child class!\n");
        return NULL;
    };
    #endif

    platform->input.wheel_delta = 0;
    platform->input.mouse_x = 0;
    platform->input.mouse_y = 0;
    

    memset(platform->input.keyboard_map, 0, sizeof(platform->input.keyboard_map));
    memset(platform->input.prev_keyboard_map, 0, sizeof(platform->input.prev_keyboard_map));
    memset(platform->input.mouse_map, 0, sizeof(platform->input.mouse_map));
    memset(platform->input.mouse_map, 0, sizeof(platform->input.prev_mouse_map));

    am_platform_set_key_callback(platform, am_platform_key_callback_default);
    am_platform_set_mouse_button_callback(platform, am_platform_mouse_button_callback_default);
    am_platform_set_mouse_motion_callback(platform, am_platform_mouse_motion_callback_default);
    am_platform_set_mouse_scroll_callback(platform, am_platform_mouse_scroll_callback_default);
    am_platform_set_window_motion_callback(platform, am_platform_window_motion_callback_default);
    am_platform_set_window_size_callback(platform, am_platform_window_size_callback_default);
    printf("[OK] Platform init successful!\n");
    return platform;
};

void am_platform_poll_events() {
    #if defined(AM_LINUX)
    am_platform *platform = am_engine_get_subsystem(platform);
    XEvent xevent;
    while (XPending(platform->display)) {
        XNextEvent(platform->display, &xevent);
        am_platform_linux_event_handler(&xevent);  
    };
    #else
    MSG msg;
    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
    };
    #endif 
};

#if defined(AM_LINUX) 
void am_platform_linux_event_handler(XEvent *xevent) {
    am_platform *platform = am_engine_get_subsystem(platform);
    am_uint64 handle = xevent->xany.window;
    switch (xevent->type) {
        case KeyPress: {
            am_key_map key = platform->input.keycodes[xevent->xkey.keycode];
            platform->callbacks.am_platform_key_callback(handle, key, AM_EVENT_KEY_PRESS);  
            break;
        };
        case KeyRelease: {
            am_key_map key = platform->input.keycodes[xevent->xkey.keycode];
            platform->callbacks.am_platform_key_callback(handle, key, AM_EVENT_KEY_RELEASE);  
            break;
        };
        case ButtonPress: {
            am_mouse_map button = am_platform_translate_button(xevent->xbutton.button);
            if (button == AM_MOUSE_BUTTON_INVALID) {
                if (xevent->xbutton.button == 4) {
                    platform->callbacks.am_platform_mouse_scroll_callback(handle, AM_EVENT_MOUSE_SCROLL_UP);
                    break;
                };
                if (xevent->xbutton.button == 5) {
                    platform->callbacks.am_platform_mouse_scroll_callback(handle, AM_EVENT_MOUSE_SCROLL_DOWN);
                    break;
                };
            };
            platform->callbacks.am_platform_mouse_button_callback(handle, button, AM_EVENT_MOUSE_BUTTON_PRESS);
            break;
        };
        case ButtonRelease: {
            am_mouse_map button = am_platform_translate_button(xevent->xbutton.button);
            platform->callbacks.am_platform_mouse_button_callback(handle, button, AM_EVENT_MOUSE_BUTTON_RELEASE);
            break;
        };
        case MotionNotify: {
            platform->callbacks.am_platform_mouse_motion_callback(handle, xevent->xbutton.x, xevent->xbutton.y, AM_EVENT_MOUSE_MOTION);
            break;
        };  
        case ConfigureNotify: {
            am_window *window = am_platform_window_lookup(handle);
            if (window->info.height != xevent->xconfigure.height || window->info.width != xevent->xconfigure.width) {
                platform->callbacks.am_platform_window_size_callback(handle, xevent->xconfigure.width, xevent->xconfigure.height, AM_EVENT_WINDOW_SIZE);
            };
            if (window->info.x != xevent->xconfigure.x || window->info.y != xevent->xconfigure.y) {
                platform->callbacks.am_platform_window_motion_callback(handle, xevent->xconfigure.x, xevent->xconfigure.y, AM_EVENT_WINDOW_MOTION);
            };
            break;
        };
        //REVIEW: Looks good, needs further testing to make sure
        case DestroyNotify: {
            printf("Destroying window %d!\n", am_platform_window_lookup(xevent->xclient.window)->am_id);
            am_dyn_array_remove(&platform->windows, am_platform_window_index_lookup(handle), 1);
            am_free(am_platform_window_lookup(xevent->xclient.window));

            am_bool check_no_root = true;
            for (am_int32 i = 0; i < platform->windows.length; i++) 
                if (am_dyn_array_data_retrieve(&platform->windows, am_window, i)->info.parent == AM_WINDOW_ROOT_PARENT) {
                    check_no_root = false;
                    break;
                };

            if (check_no_root) am_engine_get_instance()->info.is_running = false;
            break;
        };
        case ClientMessage: {
            if (xevent->xclient.data.l[0] == XInternAtom(platform->display, "WM_DELETE_WINDOW", false)) {
                XUnmapWindow(platform->display, handle);
                XDestroyWindow(platform->display, handle);
            };
            break;
        };
        default: break;
    };
};
#else
LRESULT CALLBACK am_platform_win32_event_handler(HWND handle, am_uint32 event, WPARAM wparam, LPARAM lparam) {
    am_platform *platform = am_engine_get_subsystem(platform);
    am_uint64 window_handle = (am_uint64) handle;
    switch (event) {
        case WM_KEYDOWN: {
            am_key_map key = platform->input.keycodes[(HIWORD(lparam) & (KF_EXTENDED | 0xff))];
            platform->callbacks.am_platform_key_callback(window_handle, key, AM_EVENT_KEY_PRESS);
            break;
        };
        case WM_KEYUP: {
            am_key_map key = platform->input.keycodes[(HIWORD(lparam) & (KF_EXTENDED | 0xff))];
            platform->callbacks.am_platform_key_callback(window_handle, key, AM_EVENT_KEY_RELEASE);
            break;
        };
        case WM_LBUTTONDOWN: {
            platform->callbacks.am_platform_mouse_button_callback(window_handle, AM_MOUSE_BUTTON_LEFT, AM_EVENT_MOUSE_BUTTON_PRESS);
            break;
        };
        case WM_LBUTTONUP: {
            platform->callbacks.am_platform_mouse_button_callback(window_handle, AM_MOUSE_BUTTON_LEFT, AM_EVENT_MOUSE_BUTTON_RELEASE);
            break;  
        };
        case WM_MBUTTONDOWN: {
            platform->callbacks.am_platform_mouse_button_callback(window_handle, AM_MOUSE_BUTTON_MIDDLE, AM_EVENT_MOUSE_BUTTON_PRESS);
            break;
        };
        case WM_MBUTTONUP: {
            platform->callbacks.am_platform_mouse_button_callback(window_handle, AM_MOUSE_BUTTON_MIDDLE, AM_EVENT_MOUSE_BUTTON_RELEASE);
            break;  
        };
        case WM_RBUTTONDOWN: {
            platform->callbacks.am_platform_mouse_button_callback(window_handle, AM_MOUSE_BUTTON_RIGHT, AM_EVENT_MOUSE_BUTTON_PRESS);
            break;
        };
        case WM_RBUTTONUP: {
            platform->callbacks.am_platform_mouse_button_callback(window_handle, AM_MOUSE_BUTTON_RIGHT, AM_EVENT_MOUSE_BUTTON_RELEASE);
            break;  
        };
        case WM_MOUSEWHEEL: {
            ++platform->input.wheel_delta;
            break;
        };
        case WM_MOUSEMOVE: {
            platform->callbacks.am_platform_mouse_motion_callback(window_handle, LOWORD(lparam), HIWORD(lparam), AM_EVENT_MOUSE_MOTION);
            break;
        };
        case WM_GETMINMAXINFO: {
            MINMAXINFO *info = (MINMAXINFO*)lparam;
            info->ptMinTrackSize.x = 1;
            info->ptMinTrackSize.y = 1;
            break;
        };
        case WM_SIZE: {
            if (am_platform_window_exists(window_handle)) 
                platform->callbacks.am_platform_window_size_callback(window_handle, LOWORD(lparam), HIWORD(lparam), AM_EVENT_WINDOW_SIZE);
            break;
        };
        case WM_MOVE: {
            if (am_platform_window_exists(window_handle)) 
                platform->callbacks.am_platform_window_motion_callback(window_handle, LOWORD(lparam), HIWORD(lparam), AM_EVENT_WINDOW_MOTION);

            break;
        };
        case WM_DESTROY: {
            am_dyn_array_remove(&platform->windows, am_platform_window_index_lookup(window_handle), 1);
            am_free(am_platform_window_lookup(window_handle));
            
            am_bool check_no_root = true;
            for (am_int32 i = 0; i < platform->windows.length; i++) 
                if (am_dyn_array_data_retrieve(&platform->windows, am_window, i)->info.parent == AM_WINDOW_ROOT_PARENT) {
                    check_no_root = false;
                    break;
                };

            //HACK: Temporary var. to quit loop, should send proper closing signal
            if (check_no_root) temp_check = false;
            break;
        };
        default: break;
    };
    return DefWindowProc(handle, event, wparam, lparam);
};
#endif

//REVIEW: Passing pointer argument is probably unnecessary, only one platform instance should exist
void am_platform_update(am_platform *platform) {
    memcpy(platform->input.prev_mouse_map, platform->input.mouse_map, sizeof(platform->input.mouse_map));
    memcpy(platform->input.prev_keyboard_map, platform->input.keyboard_map, sizeof(platform->input.keyboard_map));
    platform->input.wheel_delta = 0;
    platform->input.mouse_moved = false;

    am_platform_poll_events();
};

void am_platform_terminate(am_platform *platform) {
    for (am_int32 i = 0; i <platform->windows.length; i++) am_platform_window_terminate(am_dyn_array_data_retrieve(&platform->windows, am_window, i));

    #if defined(AM_LINUX)
    //This sends the proper closing xevents
    am_platform_update(am_engine_get_subsystem(platform));
    #else
    UnregisterClass(AM_ROOT_WIN_CLASS, GetModuleHandle(NULL));
    UnregisterClass(AM_CHILD_WIN_CLASS, GetModuleHandle(NULL));
    #endif
    
    am_dyn_array_cleanup(&platform->windows);
    am_free(platform);
};

void am_platform_key_press(am_key_map key) {
    if (key >= AM_KEYCODE_COUNT) return;
    am_platform *platform = am_engine_get_subsystem(platform);
    platform->input.keyboard_map[key] = true;
};

void am_platform_key_release(am_key_map key) {
    if (key >= AM_KEYCODE_COUNT) return;
    am_platform *platform = am_engine_get_subsystem(platform);
    platform->input.keyboard_map[key] = false; 
}; 

am_bool am_platform_key_pressed(am_key_map key) {
    if (key >= AM_KEYCODE_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return platform->input.keyboard_map[key] && !platform->input.prev_keyboard_map[key]; 
}; 

am_bool am_platform_key_down(am_key_map key) {
    if (key >= AM_KEYCODE_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return platform->input.keyboard_map[key] && platform->input.prev_keyboard_map[key];
}; 

am_bool am_platform_key_released(am_key_map key) {
    if (key >= AM_KEYCODE_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return !platform->input.keyboard_map[key] && platform->input.prev_keyboard_map[key]; 
}; 

am_bool am_platform_key_up(am_key_map key) {
    if (key >= AM_KEYCODE_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return !platform->input.keyboard_map[key];
};

void am_platform_mouse_button_press(am_mouse_map button) {
    if (button >= AM_MOUSE_BUTTON_COUNT) return;
    am_platform *platform = am_engine_get_subsystem(platform);
    platform->input.mouse_map[button] = true;
}; 

void am_platform_mouse_button_release(am_mouse_map button) {
    if (button >= AM_MOUSE_BUTTON_COUNT) return;
    am_platform *platform = am_engine_get_subsystem(platform);
    platform->input.mouse_map[button] = false;

};

am_bool am_platform_mouse_button_pressed(am_mouse_map button) {
    if (button >= AM_MOUSE_BUTTON_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return platform->input.mouse_map[button] && !platform->input.prev_mouse_map[button];
};

am_bool am_platform_mouse_button_down(am_mouse_map button) {
    if (button >= AM_MOUSE_BUTTON_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return platform->input.mouse_map[button] && platform->input.prev_mouse_map[button];
};

am_bool am_platform_mouse_button_released(am_mouse_map button) {
    if (button >= AM_MOUSE_BUTTON_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return !platform->input.mouse_map[button] && platform->input.prev_mouse_map[button];
};

am_bool am_platform_mouse_button_up(am_mouse_map button) {
    if (button >= AM_MOUSE_BUTTON_COUNT) return false;
    am_platform *platform = am_engine_get_subsystem(platform);
    return !platform->input.mouse_map[button];  
};

void am_platform_mouse_position(am_int32 *x, am_int32 *y) {
    am_platform *platform = am_engine_get_subsystem(platform);
    *x = platform->input.mouse_x;
    *y = platform->input.mouse_y;
};

am_vec2 am_platform_mouse_positionv() {
    am_platform *platform = am_engine_get_subsystem(platform);
    am_vec2 posv;
    posv.x = (am_float32)platform->input.mouse_x;
    posv.y = (am_float32)platform->input.mouse_y;
    return posv;
};

am_int32 am_platform_mouse_wheel_delta() {
    am_platform *platform = am_engine_get_subsystem(platform);
    return platform->input.wheel_delta;
};

am_bool am_platform_mouse_moved() {
    am_platform *platform = am_engine_get_subsystem(platform);
    return platform->input.mouse_moved;
};

void am_platform_key_callback_default(am_uint64 window_handle, am_key_map key, am_events event) {
    am_platform *platform = am_engine_get_subsystem(platform);
    switch (event) {
        case AM_EVENT_KEY_PRESS: {
            platform->input.keyboard_map[key] = true;
            break;
        };
        case AM_EVENT_KEY_RELEASE: {
            platform->input.keyboard_map[key] = false;
            break;
        };
        default: break;
    };
};

void am_platform_mouse_button_callback_default(am_uint64 window_handle, am_mouse_map button, am_events event) {
    am_platform *platform = am_engine_get_subsystem(platform);
    switch (event) {
        case AM_EVENT_MOUSE_BUTTON_PRESS: {
            platform->input.mouse_map[button] = true;
            break;
        };
        case AM_EVENT_MOUSE_BUTTON_RELEASE: {
            platform->input.mouse_map[button] = false;
            break;
        };
        default: break;
    };
};

void am_platform_mouse_motion_callback_default(am_uint64 window_handle, am_int32 x, am_int32 y, am_events event) {
    am_platform *platform = am_engine_get_subsystem(platform);
    platform->input.mouse_moved = true;
    platform->input.mouse_x = x;
    platform->input.mouse_y = y;
};

void am_platform_mouse_scroll_callback_default(am_uint64 window_handle, am_events event) {
    am_platform *platform = am_engine_get_subsystem(platform);
    switch (event) {
        case AM_EVENT_MOUSE_SCROLL_UP: {
            ++platform->input.wheel_delta;
            break;
        };
        case AM_EVENT_MOUSE_SCROLL_DOWN: {
            --platform->input.wheel_delta;
            break;
        };
        default: break;
    };
};

void am_platform_window_size_callback_default(am_uint64 window_handle, am_uint32 width, am_uint32 height, am_events event) {
    am_window *window = am_platform_window_lookup(window_handle);

    window->cache.width = window->info.width;
    window->cache.height = window->info.height;
    window->info.width = width;
    window->info.height = height;
    printf("Window size callback: %d %d\n", width, height);
};

void am_platform_window_motion_callback_default(am_uint64 window_handle, am_uint32 x, am_uint32 y, am_events event) {
    am_window *window = am_platform_window_lookup(window_handle);
    window->cache.x = window->info.x;
    window->cache.y = window->info.y;
    window->info.x = x;
    window->info.y = y;
    printf("Window pos callback: %d %d\n", x, y);
};

//TODO: Checks for mem alloc and window creation on Windows
am_window *am_platform_window_create(am_window_info window_info) {
    am_platform *platform = am_engine_get_subsystem(platform);

    am_window *new_window = (am_window*)malloc(sizeof(am_window)); //Alloc to push then free since it's stored in the dynamic array now
    if (new_window == NULL) {
        printf("[FAIL] Could not allocate memory for window!\n");
        return NULL;
    };

    new_window->am_id = (am_int32)platform->windows.length;
    am_dyn_array_push(&platform->windows, new_window, 1);
    am_int32 dyn_id = new_window->am_id; //Save id to locate after free
    am_free(new_window);
    new_window = am_dyn_array_data_retrieve(&platform->windows, am_window, dyn_id);

    #if defined(AM_LINUX)
    XSetWindowAttributes window_attributes;
    am_int32 attribs[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    new_window->visual_info = glXChooseVisual(platform->display, 0, attribs);
    new_window->colormap = XCreateColormap(platform->display, window_info.parent, new_window->visual_info->visual, AllocNone);
    window_attributes.colormap = new_window->colormap;
    window_attributes.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |ButtonPressMask | ButtonReleaseMask | PointerMotionMask | FocusChangeMask | StructureNotifyMask | EnterWindowMask | LeaveWindowMask;
    am_uint64 window = (am_uint64)XCreateWindow(platform->display, window_info.parent, window_info.x, window_info.y, window_info.width, window_info.height, 0, new_window->visual_info->depth, InputOutput, new_window->visual_info->visual, CWColormap | CWEventMask, &window_attributes);
    if (window == BadAlloc || window == BadColor || window == BadCursor || window == BadMatch || window == BadPixmap || window == BadValue || window == BadWindow) {
        printf("[FAIL] Could not create window!\n");
        return NULL;
    };
    new_window->handle = window;
    
    Atom wm_delete = XInternAtom(platform->display, "WM_DELETE_WINDOW", true);
    XSetWMProtocols(platform->display, (Window)new_window->handle, &wm_delete, 1);
    XStoreName(platform->display, (Window)new_window->handle, window_info.title);
    XMapWindow(platform->display, (Window)new_window->handle);

    #else
	DWORD dwExStyle = WS_EX_LEFT; // 0
    DWORD dwStyle = WS_OVERLAPPED; // 0
    if (window_info.parent == AM_WINDOW_ROOT_PARENT) {
	    dwStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME;
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    };

    RECT window_rect = {
        .left = 0,
        .right = window_info.window_width,
        .top = 0,
        .bottom = window_info.window_height
    };
    AdjustWindowRectEx(&window_rect, dwStyle, false, dwExStyle);
    am_int32 rect_width = window_rect.right - window_rect.left;
    am_int32 rect_height = window_rect.bottom - window_rect.top;

    new_window->handle = (am_uint64)CreateWindowEx(dwExStyle, window_info.parent == AM_WINDOW_ROOT_PARENT ? AM_ROOT_WIN_CLASS:AM_CHILD_WIN_CLASS, window_info.window_title, dwStyle, window_info.window_x, window_info.window_y, rect_width, rect_height, NULL, NULL, GetModuleHandle(NULL), NULL);
    if (new_window->handle == 0) {
        printf("[FAIL] Could not create window!\n");
        return NULL;
    };
    if ((window_info.parent != AM_WINDOW_ROOT_PARENT)) {
        SetParent((HWND)new_window->handle, (HWND)window_info.parent);
        SetWindowLong((HWND)new_window->handle, GWL_STYLE, 0);
    };
    ShowWindow((HWND)new_window->handle, 1);
    #endif
    //TODO: Fixed Windows, test on Linux although this should be fine
    new_window->info.x = window_info.x;
    new_window->info.y = window_info.y;

    new_window->info.is_fullscreen = false;
    new_window->info.parent = window_info.parent;
    am_platform_window_fullscreen(new_window->handle, window_info.is_fullscreen);
    new_window->info = window_info;

    //REVIEW: All contexts share the data
    new_window->context = NULL;
    am_window *main_window = am_platform_window_retrieve(0);
    new_window->context = glXCreateContext(platform->display, new_window->visual_info, main_window->context/*NULL*/, GL_TRUE);
    glXMakeCurrent(platform->display, new_window->handle, new_window->context);

    return new_window;
};

am_window *am_platform_window_lookup(am_uint64 handle) {
    am_platform *platform = am_engine_get_subsystem(platform);
    for (am_int32 i = 0; i < platform->windows.length; i++)
        if (am_dyn_array_data_retrieve(&platform->windows, am_window, i)->handle == handle) 
            return am_dyn_array_data_retrieve(&platform->windows, am_window, i);
    return NULL;
};

am_int32 am_platform_window_index_lookup(am_uint64 handle) {
    am_platform *platform = am_engine_get_subsystem(platform);
    for (am_int32 i = 0; i < platform->windows.length; i++)
        if (am_dyn_array_data_retrieve(&platform->windows, am_window, i)->handle == handle)
            return i;
    return -1;
};

am_window *am_platform_window_retrieve(am_uint32 index) {
    am_platform *platform = am_engine_get_subsystem(platform);
    return am_dyn_array_data_retrieve(&platform->windows, am_window, index);
};

am_bool am_platform_window_exists(am_uint64 handle) {
    am_platform *platform = am_engine_get_subsystem(platform);
    am_bool temp = false;
    for (am_uint32 i = 0; i < platform->windows.length; i++) 
        if (handle == am_dyn_array_data_retrieve(&platform->windows, am_window, i)->handle) {
            temp = true;
            break;
    };
    return temp;
};

void am_platform_window_resize(am_uint64 handle, am_uint32 width, am_uint32 height) {
    am_window *window = am_platform_window_lookup(handle);
    window->cache.width = window->info.width;
    window->cache.height = window->info.height;
    #if defined(AM_LINUX)
    am_platform *platform = am_engine_get_subsystem(platform);
    XResizeWindow(platform->display, handle, width, height);
    #else
    RECT rect = {
        .left = 0,
        .top = 0,
        .bottom = height,
        .right = width
    };
    if (window->info.parent == AM_WINDOW_ROOT_PARENT) 
        AdjustWindowRectEx(&rect, WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME, false, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
    else
        AdjustWindowRectEx(&rect, 0, false, 0);

    SetWindowPos((HWND)handle, 0, 0, 0 , rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE | SWP_FRAMECHANGED);
    #endif
};

void am_platform_window_move(am_uint64 handle, am_uint32 x, am_uint32 y) {
    am_window *window = am_platform_window_lookup(handle);

    window->cache.x = window->info.x;
    window->cache.y = window->info.y;

    #if defined(AM_LINUX)
    am_platform *platform = am_engine_get_subsystem(platform);
    XMoveWindow(platform->display, handle, x, y);
    #else
    RECT rect = {
        .left = x,
        .top = y,
        .bottom = window->info.window_height,
        .right = window->info.window_width
    };
    if (window->info.parent == AM_WINDOW_ROOT_PARENT) 
        AdjustWindowRectEx(&rect, WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_THICKFRAME, false, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
    else
        AdjustWindowRectEx(&rect, 0, false, 0);
    SetWindowPos((HWND)handle, 0, rect.left, rect.top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_FRAMECHANGED);
    #endif  
};

//REVIEW: Child windows could go "fullscreen" by taking the parent's client dimensions
void am_platform_window_fullscreen(am_uint64 handle, am_bool state) {
    am_window *window = am_platform_window_lookup(handle);
    if (window->info.is_fullscreen == state || window->info.parent != AM_WINDOW_ROOT_PARENT) return;

    //From here the state has changed and it is a main window
    window->cache.is_fullscreen = window->info.is_fullscreen;
    window->info.is_fullscreen = state;
    am_window_info temp_info = window->info;
    am_window_info temp_cache = window->cache;

    #if defined(AM_LINUX)
    am_platform *platform = am_engine_get_subsystem(platform);
    Atom wm_state = XInternAtom(platform->display, "_NET_WM_STATE", false);
    Atom wm_fs = XInternAtom(platform->display, "_NET_WM_STATE_FULLSCREEN", false);
    XEvent xevent = {0};
    xevent.type = ClientMessage;
    xevent.xclient.window = window->handle;
    xevent.xclient.message_type = wm_state;
    xevent.xclient.format = 32;
    xevent.xclient.data.l[0] = state ? 1:0;
    xevent.xclient.data.l[1] = wm_fs;
    xevent.xclient.data.l[3] = 0l;
    XSendEvent(platform->display, AM_WINDOW_ROOT_PARENT, false, SubstructureRedirectMask | SubstructureNotifyMask, &xevent);
    XFlush(platform->display);
    XWindowAttributes window_attribs = {0};
    XGetWindowAttributes(platform->display, handle, &window_attribs);
    printf("Fullscreen toggle\n Pos: %d %d\n Size: %d %d\n Fullscreen toggle end\n\n", window_attribs.x, window_attribs.y, window_attribs.width, window_attribs.height);
    memcpy(&window->cache, &temp_info, sizeof(am_window_info));


    #else
    DWORD dw_style = GetWindowLong((HWND)handle, GWL_STYLE);
    if (window->info.is_fullscreen) {
        printf("Going fullscreen\n");
        MONITORINFO monitor_info = {sizeof(monitor_info)};
        GetMonitorInfo(MonitorFromWindow((HWND)handle, MONITOR_DEFAULTTONEAREST), &monitor_info);
        SetWindowLong((HWND)handle, GWL_STYLE, dw_style & ~WS_OVERLAPPEDWINDOW);
        SetWindowPos((HWND)handle, HWND_TOP, monitor_info.rcMonitor.left, monitor_info.rcMonitor.top, monitor_info.rcMonitor.right - monitor_info.rcMonitor.left, monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top, SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        memcpy(&window->cache, &temp_info, sizeof(am_window_info));
    } else {
        printf("Going not fullscreen\n");
        SetWindowLong((HWND)handle, GWL_STYLE, dw_style | WS_OVERLAPPEDWINDOW);
        am_platform_window_resize(handle, temp_cache.window_width, temp_cache.window_height);
        am_platform_window_move(handle, temp_cache.window_x, temp_cache.window_y);
        
    };
    #endif

};

void am_platform_window_terminate(am_window *window) {
    #if defined(AM_LINUX)
    am_platform *platform = am_engine_get_subsystem(platform);
    XUnmapWindow(platform->display, window->handle);
    XFreeColormap(platform->display, window->colormap);
    XFree(window->visual_info);
    glXMakeCurrent(platform->display, window->handle, NULL);
    glXDestroyContext(platform->display, window->context);
    XDestroyWindow(platform->display, window->handle);
    #else
    DestroyWindow((HWND)(window->handle));
    #endif
};

void am_platform_timer_create() {
    am_platform *platform = am_engine_get_subsystem(platform);
    #if defined(AM_LINUX)
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    platform->time.offset = (am_uint64)ts.tv_sec * (am_uint64)1000000000 + (am_uint64)ts.tv_nsec;

    platform->time.frequency = 1000000000;
    #else
    QueryPerformanceFrequency((LARGE_INTEGER*)&platform->time.frequency);
    QueryPerformanceCounter((LARGE_INTEGER*)&platform->time.offset);
    #endif
};

void am_platform_timer_sleep(am_float32 ms) {
    #if defined(AM_LINUX)
    usleep(ms*1000.0f);
    #else
    timeBeginPeriod(1);
    Sleep((uint64_t)ms);
    timeEndPeriod(1);
    #endif
};

am_uint64 am_platform_timer_value() {
    #if defined(AM_LINUX)
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (am_uint64)ts.tv_sec * (am_uint64)1000000000 + (am_uint64)ts.tv_nsec;
    #else
    am_uint64 value;
    QueryPerformanceCounter((LARGE_INTEGER*)&value);
    return value;
    #endif
};

am_uint64 am_platform_elapsed_time() {
    am_platform *platform = am_engine_get_subsystem(platform);
    return (am_platform_timer_value() - platform->time.offset);
};

//----------------------------------------------------------------------------//
//                              END PLATFORM IMPL                             //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                                START GL IMPL                               //
//----------------------------------------------------------------------------//


amgl_shader *amgl_shader_create(amgl_shader_info info) {
    am_uint32 main_shader = glCreateProgram();
    //REVIEW: Is this a good option or would malloc be better?
    am_uint32 shader_list[info.num]; 

    for (am_int32 i = 0; i < info.num; i++) {
        am_uint32 shader = 0;
        switch (info.sources[i].type) {
            case AMGL_SHADER_FRAGMENT: {
                shader = glCreateShader(GL_FRAGMENT_SHADER);
                break;
            };
            case AMGL_SHADER_VERTEX: {
                shader = glCreateShader(GL_VERTEX_SHADER);
                break;
            };
            default: break;
        };
        glShaderSource(shader, 1, (const GLchar**)&info.sources[i].source, NULL);
        glCompileShader(shader);
        shader_list[i] = shader;// For detaching after linking

        am_int32 compile_result = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);
        if (!compile_result) {
            am_int32 length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            char err_log[length];
            glGetShaderInfoLog(shader, length, &length, err_log);
            printf("[FAIL] Shader compilation failed for index %u:\n%s\n", i, err_log);
            glDeleteShader(shader);
        };
        glAttachShader(main_shader, shader);
    };

    glLinkProgram(main_shader);

    am_int32 is_linked = 0;
    glGetProgramiv(main_shader, GL_LINK_STATUS, &is_linked);
    if (!is_linked) {
        am_int32 length = 0;
        glGetProgramiv(main_shader, GL_INFO_LOG_LENGTH, &length);
        char err_log[length];
        glGetProgramInfoLog(main_shader, length, &length, err_log);
        printf("[FAIL] Failed to link program: \n%s\n", err_log);
        glDeleteProgram(main_shader);
        return NULL;
    };

    for (am_int32 i = 0; i < info.num; i++) { 
        glDetachShader(main_shader, shader_list[i]);
        glDeleteShader(shader_list[i]);
    };

    am_engine *engine = am_engine_get_instance();
    amgl_shader *ret = (amgl_shader*)malloc(sizeof(amgl_shader));
    if (ret == NULL) {
        printf("[FAIL] Could not allocate memory for shader!\n");
        return NULL;  
    };
    ret->am_id = engine->ctx_data.shaders.length;
    am_dyn_array_push(&engine->ctx_data.shaders, ret, 1);
    am_int32 dyn_id = ret->am_id;
    am_free(ret);
    ret = am_dyn_array_data_retrieve(&engine->ctx_data.shaders, amgl_shader, dyn_id);
    ret->handle = main_shader;
    ret->info = info;
    return ret;
};

//TODO: Windows impl if necessary
void amgl_shader_load_from_file(const char *path, amgl_shader_source_info *info) {
    FILE *source = fopen(path, "rb");
    am_int32 rd_size = 0;
    char* buffer = NULL;
    if (source) {
        struct stat st;
        stat(path, &st);
        rd_size = st.st_size;
        buffer = (char*)malloc(rd_size+1);
        if (buffer) fread(buffer, 1, rd_size, source);
        buffer[rd_size] = '\0';
    };
    fclose(source);
    amgl_shader_source_load_from_memory((const void*) buffer, info, rd_size);

};

//TODO: Windows impl if necessary
void amgl_shader_source_load_from_memory(const void *memory, amgl_shader_source_info *info, size_t size) {
    info->source = NULL;
    info->source = memory;
    if (info->source == NULL) {
        printf("[FAIL] Failed to load shader with amgl_shader_source_info @ %p\n", info);
        return;
    };
};

//TODO: Check if this is ok
void amgl_shader_destroy(amgl_shader *shader) {
    am_engine *engine = am_engine_get_instance();
    glDeleteProgram(shader->handle);
    am_dyn_array_remove(&engine->ctx_data.shaders, shader->am_id, 1);
};

//TODO: Windows impl if necessary
amgl_texture *amgl_texture_create(amgl_texture_info info) {
    am_engine *engine = am_engine_get_instance();

    amgl_texture *texture = (amgl_texture*)malloc(sizeof(amgl_texture));
    if (texture == NULL) {
        printf("[FAIL] Could not allocate memory for texture!\n");
        return NULL;
    };
    texture->am_id = engine->ctx_data.textures.length;
    am_dyn_array_push(&engine->ctx_data.textures, texture, 1);
    am_int32 dyn_id = texture->am_id;
    am_free(texture);
    texture = am_dyn_array_data_retrieve(&engine->ctx_data.textures, amgl_texture, dyn_id);
    
    glGenTextures(1, &texture->handle);
    glBindTexture(GL_TEXTURE_2D, texture->handle);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.width, info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.data);
    
    switch (info.format) {
        case GL_ALPHA: glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, info.width, info.height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, info.data); break;
        case GL_RED: glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, info.width, info.height, 0, GL_RED, GL_UNSIGNED_BYTE, info.data); break;
        case GL_RGB8: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, info.width, info.height, 0, GL_RGB8, GL_UNSIGNED_BYTE, info.data); break;
        case GL_RGBA8: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, info.width, info.height, 0, GL_RGBA8, GL_UNSIGNED_BYTE, info.data); break;
        case GL_RGBA16F: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, info.width, info.height, 0, GL_RGBA16, GL_FLOAT, info.data); break;
        case GL_RGBA: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.width, info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, info.data); break;
        case GL_RGBA32F: glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, info.width, info.height, 0, GL_RGBA32F, GL_FLOAT, info.data); break;
        case GL_DEPTH_COMPONENT: glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, info.width, info.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, info.data); break;
        case GL_DEPTH_COMPONENT16: glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, info.width, info.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, info.data); break;
        case GL_DEPTH_COMPONENT24: glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, info.width, info.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, info.data); break;
        case GL_DEPTH_COMPONENT32F: glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, info.width, info.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, info.data); break;
        case GL_DEPTH24_STENCIL8: glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, info.width, info.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, info.data); break;
        case GL_DEPTH32F_STENCIL8: glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, info.width, info.height, 0, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, info.data); break;
        default: break;
    }

    info.min_filter = info.min_filter == GL_NEAREST ? GL_NEAREST:GL_LINEAR;
    info.mag_filter = info.mag_filter == GL_NEAREST ? GL_NEAREST:GL_LINEAR;

    if (info.mip_num) {
        if (info.min_filter == GL_NEAREST) 
            info.min_filter = info.mip_filter == GL_NEAREST_MIPMAP_NEAREST ? GL_NEAREST_MIPMAP_NEAREST:GL_NEAREST_MIPMAP_LINEAR;
        else
            info.min_filter = info.mip_filter == GL_LINEAR_MIPMAP_NEAREST ? GL_LINEAR_MIPMAP_NEAREST:GL_LINEAR_MIPMAP_LINEAR;     

        glGenerateMipmap(GL_TEXTURE_2D);
    };

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, info.min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, info.mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, info.wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, info.wrap_t);

    //HACK
    //TODO: Research if this is needed for every texture, might not be needed if using shaders as far as I understand
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    texture->info = info;
    return texture;
};

//TODO: Windows impl if necessary
void amgl_texture_load_from_file(const char *path, amgl_texture_info *info, am_bool flip) {
    //STORE IMAGE INTO MEMORY
    FILE *file = fopen(path, "rb");
    char* buffer = NULL;
    size_t rd_size = 0;

    if (file) {
        struct stat st;
        stat(path, &st);
        rd_size = st.st_size;
        buffer = (char*)malloc(rd_size+1);
        if (buffer) fread(buffer, 1, rd_size, file);
        buffer[rd_size] = '\0'; 
    };
    fclose(file);
    amgl_texture_load_from_memory(buffer, info, rd_size, flip);
};

//TODO: Windows impl if necessary
void amgl_texture_load_from_memory(const void *memory, amgl_texture_info *info, size_t size, am_bool flip) {
    am_int32 num_comps = 0;
    stbi_set_flip_vertically_on_load(flip);
    info->data = NULL;
    info->data = (void*)stbi_load_from_memory((const stbi_uc*)memory, size, (am_int32*)&info->width, (am_int32*)&info->height, &num_comps, 4);
    if (info->data == NULL) {
        printf("[FAIL] Unable to load texture with am_texture_info @ %p\n", info); //Prints place in memory
        return;
    };
};

//TODO: Check if this is ok
void amgl_texture_destroy(amgl_texture *texture) {
    am_engine *engine = am_engine_get_instance();
    glDeleteTextures(1, &texture->handle);
    am_dyn_array_remove(&engine->ctx_data.textures, texture->am_id, 1);
};

//Create arrays for shaders, vertex b, index b, frame b etc, init gl addresses
//TODO: Windows impl if necessary
void amgl_init() {
    //TODO: Init all gl procedures and functions
    glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)amgl_get_proc_address("glXSwapIntervalEXT");
    glCreateShader = (PFNGLCREATESHADERPROC)amgl_get_proc_address("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)amgl_get_proc_address("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)amgl_get_proc_address("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)amgl_get_proc_address("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)amgl_get_proc_address("glGetShaderInfoLog");
    glDetachShader = (PFNGLDETACHSHADERPROC)amgl_get_proc_address("glDetachShader");
    glDeleteShader = (PFNGLDELETESHADERPROC)amgl_get_proc_address("glDeleteShader");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)amgl_get_proc_address("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)amgl_get_proc_address("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)amgl_get_proc_address("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)amgl_get_proc_address("glGetProgramiv");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)amgl_get_proc_address("glGetProgramInfoLog");
    glDeleteProgram = (PFNGLDELETEPROGRAMPROC)amgl_get_proc_address("glDeleteProgram");
    glUseProgram = (PFNGLUSEPROGRAMPROC)amgl_get_proc_address("glUseProgram");
    glGenBuffers = (PFNGLGENBUFFERSPROC)amgl_get_proc_address("glGenBuffers");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)amgl_get_proc_address("glDeleteBuffers");
    glBindBuffer = (PFNGLBINDBUFFERPROC)amgl_get_proc_address("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)amgl_get_proc_address("glBufferData");
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)amgl_get_proc_address("glGenVertexArrays");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)amgl_get_proc_address("glDeleteVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)amgl_get_proc_address("glBindVertexArray");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)amgl_get_proc_address("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)amgl_get_proc_address("glEnableVertexAttribArray");
    glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)amgl_get_proc_address("glGetAttribLocation");
    glUniform1i = (PFNGLUNIFORM1IPROC)amgl_get_proc_address("glUniform1i");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)amgl_get_proc_address("glGetUniformLocation");
    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)amgl_get_proc_address("glGenerateMipmap");
};

//TODO: Windows impl if necessary
void amgl_terminate() {

};

//TODO: Windows impl if necessary
void amgl_set_viewport(am_int32 x, am_int32 y, am_int32 width, am_int32 height) {
    glViewport(x, y, width, height);
};

//TODO: Windows impl if necessary
void amgl_vsync(am_window *window, am_bool state) {
    am_platform *platform = am_engine_get_subsystem(platform);
    glXSwapIntervalEXT(platform->display, window->handle, state);
};


//----------------------------------------------------------------------------//
//                                 END GL IMPL                                //
//----------------------------------------------------------------------------//


//----------------------------------------------------------------------------//
//                              START ENGINE IMPL                             //
//----------------------------------------------------------------------------//


void am_engine_create(am_engine_info engine_info){
    am_engine_get_instance() = (am_engine*)malloc(sizeof(am_engine));
    am_engine *engine = am_engine_get_instance();
    engine->info = engine_info;

    engine->platform = am_platform_create();
    am_platform_timer_create();
    amgl_init();

    //TODO: Init all ctx_data arrays
    am_dyn_array_init(&engine->ctx_data.textures, sizeof(amgl_texture));
    am_dyn_array_init(&engine->ctx_data.shaders, sizeof(amgl_shader));

    printf("[OK] Before window create successful!\n");
    am_window_info main = {
        .height = engine->info.initial_height,
        .width = engine->info.initial_width,
        .title = engine->info.initial_title,
        .x = engine->info.initial_x,
        .y = engine->info.initial_y,
        .parent = AM_WINDOW_ROOT_PARENT,
        .is_fullscreen = engine->info.initial_fullscreen
    };
    am_platform_window_create(main);
    printf("[OK] Window create successful!\n");
    #if defined(AM_LINUX)
    //XSetWindowBackground(am_engine_get_subsystem(platform)->display, main_wind->handle, 0x0000FF);
    #endif

    amgl_set_viewport(0,0, (am_int32)main.width-10, (am_int32)main.height-10);
    //TODO: Temporary
    engine->info.is_running = true;
    printf("[OK] Engine create successful!\n");
};

void am_engine_terminate(){
    am_engine *engine = am_engine_get_instance();
    am_platform_terminate(am_engine_get_subsystem(platform));

    am_dyn_array_cleanup(&engine->ctx_data.textures);
    am_dyn_array_cleanup(&engine->ctx_data.shaders);

    amgl_terminate();
    //TODO: Temporary
    engine->info.is_running = false;
    am_free(engine);
};

//----------------------------------------------------------------------------//
//                               END ENGINE IMPL                              //
//----------------------------------------------------------------------------//


int main() {
    am_engine_info eng_inf = {
    .initial_title = "Main",
    .initial_fullscreen = false,
    .initial_width = 500,
    .initial_height = 500,
    .initial_x = 100,
    .initial_y = 100
    };
    am_engine_create(eng_inf);
    /*
    am_window_info childw = {
        .height = 100,
        .width = 100,
        .x = 70,
        .y = 70,
        .parent = am_platform_window_retrieve(0)->handle,
        .is_fullscreen = false
    };
    am_window *ccc = am_platform_window_create(childw);
    XSetWindowBackground(am_engine_get_subsystem(platform)->display, ccc->handle, 0xFF0000);*/

    am_platform *platform = am_engine_get_subsystem(platform);
    am_bool run = true;
    am_uint64 t = 0;
    amgl_texture_info texinfo = {
        .format = GL_RGBA,
        .mag_filter = GL_LINEAR,
        .min_filter = GL_LINEAR,
        .wrap_s = GL_REPEAT,
        .wrap_t = GL_REPEAT
    };
    amgl_texture_load_from_file("/home/truta/Downloads/color_test.png", &texinfo, true);
    amgl_texture *test = amgl_texture_create(texinfo);

    //Load shader source
    amgl_shader_source_info shader_src = {
        .type = AMGL_SHADER_VERTEX
    };
    amgl_shader_load_from_file("test_v.glsl", &shader_src);
    printf("TEST SHADER %s\n", shader_src.source);

    //Create shader info struct & shader
    amgl_shader_info test_shader= {
        .num = 1,
        .sources = &shader_src
    };
    amgl_shader *tt = amgl_shader_create(test_shader);

    glXMakeCurrent(platform->display, am_platform_window_retrieve(0)->handle, am_platform_window_retrieve(0)->context);
    printf("CHECK POINTERS AT DYN ARRAY TEXTURES (TEXTURE0): %p vs %p\n", am_dyn_array_data_retrieve(&am_engine_get_instance()->ctx_data.textures, amgl_texture, 0), am_engine_get_instance()->ctx_data.textures.data);
    while(run) {
        t++;/*
        if (t % 2 == 1) glXMakeCurrent(platform->display, am_platform_window_retrieve(0)->handle, am_platform_window_retrieve(0)->context);
        else glXMakeCurrent(platform->display, am_platform_window_retrieve(1)->handle, am_platform_window_retrieve(1)->context);*/
        //DOES WORK
        glClearColor(1,1,0,0);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindTexture(GL_TEXTURE_2D, am_dyn_array_data_retrieve(&am_engine_get_instance()->ctx_data.textures, amgl_texture, 0)->handle);
        glBegin(GL_TRIANGLES);
        //glColor3f(0.1, 0.2, 0.3);
        glTexCoord2f(0, 0); glVertex3f( -1,  0, -1 );
        glTexCoord2f(1, 0); glVertex3f(  1,  0, -1 );
        glTexCoord2f(0, 1); glVertex3f( -1,  1, -1 );
        // second triangle, top right half
        glTexCoord2f(1, 0); glVertex3f(  1,  0, -1 );
        glTexCoord2f(0, 1); glVertex3f( -1,  1, -1 );
        glTexCoord2f(1, 1); glVertex3f(  1,  1, -1 );
        glEnd();
        glFlush();
        glXSwapBuffers(platform->display, am_platform_window_retrieve(0)->handle);
        //if (t % 2 == 1) glXSwapBuffers(platform->display, am_platform_window_retrieve(0)->handle);
        //else glXSwapBuffers(platform->display, am_platform_window_retrieve(1)->handle);
        
        am_platform_update(am_engine_get_subsystem(platform));
        if (am_platform_key_released(AM_KEYCODE_X)) run = false;
    };
    am_platform_terminate(am_engine_get_subsystem(platform));
    getchar();
    return 0;
};
