#pragma once

typedef void (*EventCallback)(void *);

typedef struct EventNode {
  int event_key;
  EventCallback callback;

  struct EventNode* prev;
  struct EventNode* next;
} EventNode;

typedef struct EventManager {
  struct EventNode* head;
  struct EventNode* tail;
} EventManager;

EventManager* event_manager_create();
void event_manager_destroy(EventManager* events);

void event_manager_subscribe(EventManager* events, int event_key, EventCallback callback);
void event_manager_unsubscribe(EventManager* events, int event_key);

int event_manager_raise_event(EventManager* events, int event_key);
int event_manager_raise_event_with_context(EventManager* events, int event_key, void* context);
