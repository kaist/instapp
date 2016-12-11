#include <pebble.h>


Window *start_window;
Window *cards_window;
Window *text_window;

TextLayer *load_text;
TextLayer *st_text;
TextLayer *like_text;
TextLayer *dates_text;
TextLayer *info_text;
TextLayer *caption_text;
static BitmapLayer *main_bitmap_layer;
static BitmapLayer *loading_bitmap_layer;
static GBitmap *main_bitmap;
static GBitmap *loading_bitmap;

static BitmapLayer *up_bitmap_layer;
static GBitmap *up_bitmap;

static BitmapLayer *down_bitmap_layer;
static GBitmap *down_bitmap;

static BitmapLayer *action_bitmap_layer;
static GBitmap *action_bitmap;









uint8_t state;
uint8_t page=0;
uint8_t loading=0;
uint8_t is_like_var=0;
char buffer [10];
char username[30];
uint16_t likes=0;
char likesb[30];
char datesb[30];
char caption[400];

static uint8_t *s_img_data;
static int s_img_size;

uint8_t cards=0;






void main_w (void)
  {

  window_stack_push(cards_window,true);
  
  
};


void main_window_set_image_data(uint8_t *data, int size) {
  if(main_bitmap) {
    gbitmap_destroy(main_bitmap);
  }

  // Create new GBitmap from downloaded PNG data
  main_bitmap = gbitmap_create_from_png_data(data, size);

  if(main_bitmap) {
    bitmap_layer_set_bitmap(main_bitmap_layer, main_bitmap);
  
  } else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error creating GBitmap from PNG data!");
  }
  free(s_img_data);
  loading=0;
}

static void send_update(uint8_t cmd) {
  if(main_bitmap) {
    gbitmap_destroy(main_bitmap);
  }
  main_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_SYNC);
  bitmap_layer_set_compositing_mode(main_bitmap_layer, GCompOpSet);

  bitmap_layer_set_bitmap(main_bitmap_layer, main_bitmap);     
  DictionaryIterator *iterator = NULL;
  if (app_message_outbox_begin(&iterator) == APP_MSG_OK && iterator != NULL) {
    dict_write_uint8(iterator,  MESSAGE_KEY_update, cmd);
    app_message_outbox_send();
    loading=1;
  
};
};

static void send_like() {
  DictionaryIterator *iterator = NULL;
  if (app_message_outbox_begin(&iterator) == APP_MSG_OK && iterator != NULL) {
    dict_write_uint8(iterator,  MESSAGE_KEY_sendlike, page);
    app_message_outbox_send();
  
};
};

static void inbox_received_handler(DictionaryIterator *iter, void *context) {
  
  Tuple *likes_tuple = dict_find(iter, MESSAGE_KEY_likes);
  if(likes_tuple) {
    likes=likes_tuple->value->uint16;
    snprintf(likesb,30, "\U0001F499 %d",likes );
    text_layer_set_text(like_text,  likesb);

  };
  Tuple *pages_tuple = dict_find(iter, MESSAGE_KEY_pages);
  if(pages_tuple) {
    cards=pages_tuple->value->uint8;


  }; 
  
  Tuple *username_tuple = dict_find(iter, MESSAGE_KEY_username);
  if(username_tuple) {
    strncpy(username, username_tuple->value->cstring, 20);
    text_layer_set_text(st_text, username);
    text_layer_set_text(info_text, username);
  };

  Tuple *caption_tuple = dict_find(iter, MESSAGE_KEY_captions);
  if(caption_tuple) {
    strncpy(caption, caption_tuple->value->cstring, 400);
    text_layer_set_text(caption_text, caption);
  };  
  
  Tuple *dates_tuple = dict_find(iter, MESSAGE_KEY_dates);
  if(dates_tuple) {
    strncpy(datesb, dates_tuple->value->cstring, 30);
    text_layer_set_text(dates_text, datesb);

  };
  
  Tuple *islike_tuple = dict_find(iter, MESSAGE_KEY_is_like);
  if(islike_tuple) {
    if (islike_tuple->value->uint8==1)
      {
      is_like_var=1;
      text_layer_set_text_color(like_text, GColorRed);
      
    }
    if (islike_tuple->value->uint8==2) {
      is_like_var=2;
      text_layer_set_text_color(like_text, GColorWhite);
      
      
    }

  }; 
  
  Tuple *state_tuple = dict_find(iter, MESSAGE_KEY_state);
  if(state_tuple) {
    state=state_tuple->value->uint8;
    if (state==1)
      {
      window_stack_remove(start_window,true);
      main_w();
      
      send_update(page);
            light_enable_interaction();
      vibes_short_pulse();

      
      
     
      
      };
    if (state==2)
      {
       window_set_background_color(start_window,GColorRed);
      text_layer_set_text(load_text, "Check app config on your smartphone \U0001F61F");
        if(loading_bitmap) {
       gbitmap_destroy(loading_bitmap);
  }
  loading_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_ERROR);
  bitmap_layer_set_compositing_mode(loading_bitmap_layer, GCompOpSet);
  bitmap_layer_set_bitmap(loading_bitmap_layer, loading_bitmap);  
    }
    
    
    
    
  
  };
  
  Tuple *img_size_t = dict_find(iter,  MESSAGE_KEY_lenght);
  if(img_size_t) {
    s_img_size = img_size_t->value->int32;

    // Allocate buffer for image data
    s_img_data = (uint8_t*)malloc(s_img_size * sizeof(uint8_t));
  }
 
  // An image chunk
  Tuple *chunk_t = dict_find(iter, MESSAGE_KEY_chunk); 
  if(chunk_t) {
    uint8_t *chunk_data = chunk_t->value->data;

    Tuple *chunk_size_t = dict_find(iter, MESSAGE_KEY_chunksize);
    int chunk_size = chunk_size_t->value->int32;

    Tuple *index_t = dict_find(iter, MESSAGE_KEY_index);
    int index = index_t->value->int32;

    // Save the chunk
    memcpy(&s_img_data[index], chunk_data, chunk_size);
  }

  // Complete?
  Tuple *complete_t = dict_find(iter, MESSAGE_KEY_complete);
  if(complete_t) {
    // Show the image
    main_window_set_image_data(s_img_data, s_img_size);
  }  

  

 
};




void down_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  
  if (state==1) {
    if (page<(cards-1) && loading==0) 
      {
      page++;
      send_update(page);
    
      
    }
    
    if (page==1) {
        up_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_UP);
        bitmap_layer_set_compositing_mode(up_bitmap_layer, GCompOpSet);
        bitmap_layer_set_bitmap(up_bitmap_layer, up_bitmap); 

    }
    if (page==(cards-1))
      {
        down_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_EMPTY);
        bitmap_layer_set_compositing_mode(down_bitmap_layer, GCompOpSet);
        bitmap_layer_set_bitmap(down_bitmap_layer, down_bitmap);     
    }
    
  }
}

void like_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (state==1 && loading==0 && is_like_var==2){
    text_layer_set_text_color(like_text, GColorRed);
    likes++;
    snprintf(likesb,30, "\U0001F499 %d",likes );
    text_layer_set_text(like_text,  likesb);
    send_like();
    
  }
    
  
};

void show_text(ClickRecognizerRef recognizer, void *context) {

   window_stack_push(text_window,true);
  
};

void hide_text(ClickRecognizerRef recognizer, void *context) {

  
};

void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (state==1) {
    if (page>0 && loading==0) 
      {
      page--;
      send_update(page);
  

      
    }
    
    if (page==0) {
        up_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_EMPTY);
        bitmap_layer_set_compositing_mode(up_bitmap_layer, GCompOpSet);
        bitmap_layer_set_bitmap(up_bitmap_layer, up_bitmap); 
    }
    
     if (page==(cards-2))
      {
        down_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_DOWN);
        bitmap_layer_set_compositing_mode(down_bitmap_layer, GCompOpSet);
        bitmap_layer_set_bitmap(down_bitmap_layer, down_bitmap);     
    };

  }
}
static void window_load(Window *window) {

  
  
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  load_text = text_layer_create(grect_inset(bounds, GEdgeInsets(20, 0)));
  text_layer_set_text(load_text, "Loading timeline...");
  text_layer_set_text_alignment(load_text, GTextAlignmentCenter);
  text_layer_set_background_color(load_text, GColorClear);
  text_layer_set_text_color(load_text, GColorWhite);
  text_layer_set_font(load_text, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(load_text));
  
  loading_bitmap_layer = bitmap_layer_create(GRect(0, 50, 144, 144)); 
  layer_add_child(window_layer, bitmap_layer_get_layer(loading_bitmap_layer));  
  
  loading_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_LOADING);
  bitmap_layer_set_compositing_mode(loading_bitmap_layer, GCompOpSet);
  bitmap_layer_set_bitmap(loading_bitmap_layer,loading_bitmap);   
}



void config_provider(Window *window) {
 // single click / repeat-on-hold config:
  window_single_click_subscribe(BUTTON_ID_DOWN, down_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, like_click_handler);
  window_long_click_subscribe(BUTTON_ID_SELECT, 500, show_text,hide_text);
}

void handle_init(void) {
  start_window = window_create();
  window_set_background_color(start_window,GColorBlueMoon);
  window_set_window_handlers(start_window, (WindowHandlers) {
    .load = window_load
  });
  window_stack_push(start_window, true);
  

  cards_window = window_create();
  window_set_background_color(cards_window,GColorDukeBlue);  
  Layer *window_layer = window_get_root_layer(cards_window);
  
  text_window = window_create();
  window_set_background_color(text_window,GColorDukeBlue);  
  Layer *text_layer = window_get_root_layer(text_window); 
  
  st_text = text_layer_create(GRect(5, 127, 125, 136));
  text_layer_set_text_alignment(st_text, GTextAlignmentLeft);
  text_layer_set_background_color(st_text, GColorClear);
  text_layer_set_text_color(st_text, GColorWhite);
  text_layer_set_font(st_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  
  like_text = text_layer_create(GRect(5, 145, 125, 168));
  text_layer_set_text_alignment(like_text, GTextAlignmentLeft);
  text_layer_set_background_color(like_text, GColorClear);
  text_layer_set_text_color(like_text, GColorWhite);
  text_layer_set_font(like_text, fonts_get_system_font(FONT_KEY_GOTHIC_14));  
  
  dates_text = text_layer_create(GRect(5, 145, 120, 168));
  text_layer_set_text_alignment(dates_text, GTextAlignmentRight);
  text_layer_set_background_color(dates_text, GColorClear);
  text_layer_set_text_color(dates_text, GColorWhite);
  text_layer_set_font(dates_text, fonts_get_system_font(FONT_KEY_GOTHIC_14)); 
  
  
  up_bitmap_layer = bitmap_layer_create(GRect(87, 15, 99, 22)); 
  layer_add_child(window_layer, bitmap_layer_get_layer(up_bitmap_layer)); 
  
  down_bitmap_layer = bitmap_layer_create(GRect(87, 95, 99, 102)); 
  layer_add_child(window_layer, bitmap_layer_get_layer(down_bitmap_layer));  
  
  
  
  main_bitmap_layer = bitmap_layer_create(GRect(5, 5, 125, 125));
  layer_add_child(window_layer, bitmap_layer_get_layer(main_bitmap_layer));
  
  action_bitmap_layer = bitmap_layer_create(GRect(87, 45, 99, 75));
  layer_add_child(window_layer, bitmap_layer_get_layer(action_bitmap_layer)); 
 

  
  layer_add_child(window_layer, text_layer_get_layer(st_text));
  layer_add_child(window_layer, text_layer_get_layer(like_text));
  layer_add_child(window_layer, text_layer_get_layer(dates_text));

  
  action_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_ACTION);
  bitmap_layer_set_compositing_mode(action_bitmap_layer, GCompOpSet);
  bitmap_layer_set_bitmap(action_bitmap_layer, action_bitmap);  
  
  up_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_EMPTY);
  bitmap_layer_set_compositing_mode(up_bitmap_layer, GCompOpSet);
  bitmap_layer_set_bitmap(up_bitmap_layer, up_bitmap); 
  
  down_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_DOWN);
  bitmap_layer_set_compositing_mode(down_bitmap_layer, GCompOpSet);
  bitmap_layer_set_bitmap(down_bitmap_layer, down_bitmap);   
  
  info_text = text_layer_create(GRect(5, 5, 120, 20));
  text_layer_set_text_alignment(info_text, GTextAlignmentLeft);
  text_layer_set_background_color(info_text, GColorClear);
  text_layer_set_text_color(info_text, GColorWhite);
  text_layer_set_font(info_text, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD)); 
  layer_add_child(text_layer, text_layer_get_layer(info_text));

  caption_text = text_layer_create(GRect(5, 30, 140, 140));
  text_layer_set_text_alignment(caption_text, GTextAlignmentLeft);
  text_layer_set_background_color(caption_text, GColorClear);
  text_layer_set_text_color(caption_text, GColorWhite);
  text_layer_set_font(caption_text, fonts_get_system_font(FONT_KEY_GOTHIC_14)); 
  layer_add_child(text_layer, text_layer_get_layer(caption_text));
  
  
  
  
  app_message_register_inbox_received(inbox_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  window_set_click_config_provider(cards_window, (ClickConfigProvider) config_provider);

}

void handle_deinit(void) {
  window_destroy(start_window);
  window_destroy(cards_window);
  window_destroy(text_window);
  
  text_layer_destroy(load_text);
  text_layer_destroy(st_text);
  text_layer_destroy(like_text);
  text_layer_destroy(dates_text);
  text_layer_destroy(info_text);
  text_layer_destroy(caption_text);
  text_layer_destroy(load_text);
  
  bitmap_layer_destroy(main_bitmap_layer);
  bitmap_layer_destroy(loading_bitmap_layer);
  bitmap_layer_destroy(up_bitmap_layer);
  bitmap_layer_destroy(down_bitmap_layer);
  bitmap_layer_destroy(action_bitmap_layer);
  
  gbitmap_destroy(main_bitmap);
  gbitmap_destroy(loading_bitmap);
  gbitmap_destroy(up_bitmap);
  gbitmap_destroy(down_bitmap);
  gbitmap_destroy(action_bitmap);
  
  
  

  
  
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
};
