#pragma once

#define SAFE_DELETE(instance) if(instance != nullptr) { delete instance; instance = nullptr;}
