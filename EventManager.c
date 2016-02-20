#include "pebble.h"
#include "EventManager.h"

#define false 0
#define true 1

static EventNode* event_node_create(int event_key, EventCallback callback) {
  // Allocate some heap space and set default value
  EventNode* this = malloc(sizeof(EventNode));
  this->event_key = event_key;
  this->callback = callback;
  this->prev = NULL;
  this->next = NULL;

  return this;
}

static EventNode* event_manager_find_event(EventManager* this, int event_key) {
  // Make sure we don't have a NULL pointer
  if (!this) return NULL;

  EventNode* event = this->head;

  while(event) {
    // Return the event if we found it
    if (event->event_key == event_key) return event;
    // Otherwise go to the next event
    event = (EventNode*)event->next;
  }

  return NULL;
}

EventManager* event_manager_create() {
  // Allocate heap space for the linked list
  EventManager* this = malloc(sizeof(EventManager));
  this->head = NULL;
  this->tail = NULL;

  return this;
}

void event_manager_destroy(EventManager* this) {
  // Make sure we don't have a NULL pointer
  if (!this) return;

  EventNode* event = this->head;
  EventNode* next;

  // Iterate over the list and destroy each event
  while(event) {
    next = (EventNode*)event->next;
    free(event);
    event = next;
  }

  // Destroy the linked list
  free(this);
}

void event_manager_subscribe(EventManager* this, int event_key, EventCallback callback) {
  // Make sure we don't have a NULL pointer
  if (!this) return;

  // If the event is already in our manager, set the callback to the new callback
  EventNode* event = event_manager_find_event(this, event_key);
  if (event) {
    event->callback = callback;
    return;
  }

  // Otherwise, create a new event and add it to the list
  event = event_node_create(event_key, callback);
  if (!this->head) {
    // If it's the first event in the list
    this->head = event;
    this->tail = event;
  } else {
    // Otherwise, add to end of list
    event->prev = this->tail;
    this->tail->next = event;
    this->tail = event;
  }
}

void event_manager_unsubscribe(EventManager* this, int event_key) {
  // Make sure we don't have a NULL pointer
  if (!this) return;

  // Find the node and return if it doesn't
  EventNode* event = event_manager_find_event(this, event_key);
  if (!event) return;

  // Make sure there's a prev node (or update head if there isn't)
  if (event == this->head) {
    this->head = event->next;
  } else {
    event->prev->next = event->next;
  }

  // Make sure there's a next node (or update tail if there isn't)
  if (event == this->tail) {
    this->tail = event->prev;
  } else {
    event->next->prev = event->prev;
  }

  // destroy the node
  free(event);
}

int event_manager_raise_event(EventManager* this, int event_key) {
  // convienience function for when we don't have data
  return event_manager_raise_event_with_context(this, event_key, NULL);
}

int event_manager_raise_event_with_context(EventManager* this, int event_key, void* context) {
  // Make sure we don't have a NULL pointer
  if (!this) return false;

  // Search for the event and return if we can't find it
  EventNode* node = event_manager_find_event(this, event_key);
  if (!node) return false;

  // Otherwise, invoke the callback and return true
  node->callback(context);
  return true;
}
