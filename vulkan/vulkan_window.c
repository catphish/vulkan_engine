#include "vulkan.h"
#include <xcb/xcb.h>
#include <xcb/xkb.h>

void vulkan_window_create(struct vulkan *vulkan) {
  uint32_t mask;
  uint32_t values[2];

  // open connection to the server
  vulkan->connection = xcb_connect(NULL,NULL);
  if (xcb_connection_has_error(vulkan->connection)) {
    ERROR("Cannot open display\n");
  }
  // get the first screen
  vulkan->screen = xcb_setup_roots_iterator(xcb_get_setup(vulkan->connection)).data;

  // create black graphics context
  vulkan->gcontext = xcb_generate_id(vulkan->connection);
  vulkan->window = vulkan->screen->root;
  mask = XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES;
  values[0] = vulkan->screen->black_pixel;
  values[1] = 0;
  xcb_create_gc(vulkan->connection, vulkan->gcontext, vulkan->window, mask, values);

  // create window
  vulkan->window = xcb_generate_id(vulkan->connection);
  mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  values[0] = vulkan->screen->black_pixel;
  values[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_POINTER_MOTION |
              XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE |
              XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE |
              XCB_EVENT_MASK_STRUCTURE_NOTIFY;

  xcb_create_window(vulkan->connection, vulkan->screen->root_depth, vulkan->window, vulkan->screen->root,
                    10, 10, 1024, 768, 1,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT, vulkan->screen->root_visual,
                    mask, values);

  xcb_xkb_use_extension(vulkan->connection, XCB_XKB_MAJOR_VERSION, XCB_XKB_MINOR_VERSION);

  //Set the per client detectable auto repeat flag
  xcb_xkb_per_client_flags(vulkan->connection,
                        XCB_XKB_ID_USE_CORE_KBD,                                                       
                        XCB_XKB_PER_CLIENT_FLAG_DETECTABLE_AUTO_REPEAT,
                        1,0,0,0);

  xcb_intern_atom_cookie_t cookie = xcb_intern_atom(vulkan->connection, 1, 12, "WM_PROTOCOLS");
  xcb_intern_atom_reply_t* reply = xcb_intern_atom_reply(vulkan->connection, cookie, 0);

  xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(vulkan->connection, 0, 16, "WM_DELETE_WINDOW");
  xcb_intern_atom_reply_t* reply2 = xcb_intern_atom_reply(vulkan->connection, cookie2, 0);
  vulkan->xcb_delete_window = (*reply2).atom;

  xcb_change_property(vulkan->connection, XCB_PROP_MODE_REPLACE, vulkan->window, (*reply).atom, 4, 32, 1, &(*reply2).atom);

  xcb_map_window(vulkan->connection, vulkan->window);
  xcb_flush(vulkan->connection);
}

uint8_t vulkan_window_poll(struct vulkan *vulkan) {
  xcb_generic_event_t* event;
  while((event = xcb_poll_for_event(vulkan->connection)))
  {
    switch((*event).response_type & ~0x80)
    {
      case XCB_CLIENT_MESSAGE:
      {
        if((*(xcb_client_message_event_t*)event).data.data32[0] == vulkan->xcb_delete_window)
        {
          return 0;
        }
        break;
      }
      default:
      {
        printf("Unknown event: %d\n", (*event).response_type);
        break;
      }
    }
    free(event);
  }
  return 1;
}
