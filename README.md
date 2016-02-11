# EventManager for Pebble

A basic library for managing event driven code in Pebble watchapps and watchfaces.

## Usage

### EventManager* event_manager_create()

The `event_manager_create` method allocates memory for a new EventManager and returns a pointer to the object.

### void event_manager_destroy(EventMnager* this)

The `event_manager_destory` method deallocates all of the memory used by the EventManager (including all associated events) passed in as the first parameter, and sets the pointer to null.

### void event_manager_subscribe(EventManager* this, int key, EventCallback callback)

The `event_manager_subscribe` method adds a new event (indicated by the *key* parameter) to the event_manager. When the event is triggered with [event_manager_raise_event](), or [event_manager_raise_event_with_context], the callback, a `void` function with a single parameter (a void pointer), will be invoked.

### void event_manager_unsubscribe(EventManager* this, int key)

The `event_manager_unsubscribe` method removes an event (indicated by the *key* parameter) from the event manager.

### bool event_manager_raise_event(EventManager* this, int key)
The `event_manager_raise_event` method results in the event associated with the *key* having it's callback invoked with a `NULL` pointer as the data.

### bool event_manager_raise_event_with_context(EventManager* this, int key, void* data)
The `event_manager_raise_event_with_context` method results in the event associated with the *key* having it's callback invoked with *data* as the data.

## Example

The following example shows how to properly allocate and deallocate memory for an event manager, subscribe to events (and write event callback handlers), as well as raise events.

This is a relatively trivial example, the EventManager library becomes much more useful for applications that span multiple windows (and files).

```
#require "pebble.h"
#require "EventManager.h"

#define ON_APP_READY 0

EventManager* event_manager;

static void on_app_ready(void* data) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "In the on_app_ready callback!");
}

void init() {
  event_manager = event_manager_create();
  event_manager_subscribe(event_manager, ON_APP_READY, on_app_ready);

  event_manager_raise_event(event_manager, ON_APP_READY);
}

void deinit() {
  event_manager_destroy(event_manager);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
```

# License

The EventManager library is licensed under the [MIT License](./LICENSE).
