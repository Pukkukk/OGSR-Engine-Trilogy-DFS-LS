#include "stdafx.h"

#include "DiscordRPC.hpp"
#include "../xr_3da/x_ray.h"

#ifndef DISABLE_DISCORD
#include "../DiscordRPC/DiscordRPC/include/discord_rpc.h"
#pragma comment(lib, "DiscordRPC")
#endif

ENGINE_API DiscordRPC Discord;

void DiscordRPC::Init()
{
#ifndef DISABLE_DISCORD
    DiscordEventHandlers nullHandlers{};
        Discord_Initialize("1484587501086572695", &nullHandlers, TRUE, nullptr);

    start_time = time(nullptr);
#endif
}

void DiscordRPC::Destroy()
{
#ifndef DISABLE_DISCORD
    Discord_ClearPresence();
    Discord_Shutdown();
#endif
}

void DiscordRPC::Update(const char* level_name_translated, const char* level_name)
{
#ifndef DISABLE_DISCORD
    DiscordRichPresence presenseInfo{};

    std::string task_txt, lname, lname_and_task;

    presenseInfo.startTimestamp = start_time; // время с момента запуска

    if (level_name)
        current_level_name = level_name;

    if (IS_OGSR_GA && current_level_name && (level_name || !level_name_translated))
        presenseInfo.largeImageKey = current_level_name; // большая картинка
    else
        presenseInfo.largeImageKey = "main_image"; // большая картинка
    static const std::string small_txt = StringHasUTF8("WILD_USEC, Кефир") ? std::string("WILD_USEC, Кефир") : StringToUTF8("WILD_USEC, Кефир");
    presenseInfo.smallImageKey = "main_image_small"; // маленькая картинка
    presenseInfo.smallImageText = small_txt.c_str(); // авторы мода на логотипе мода

    if (active_task_text)
    {
        task_txt = StringHasUTF8(active_task_text) ? active_task_text : StringToUTF8(active_task_text);
        presenseInfo.state = task_txt.c_str(); // активное задание
    }

    if (level_name_translated)
        current_level_name_translated = level_name_translated;

    if (current_level_name_translated)
    {
        lname = StringHasUTF8(current_level_name_translated) ? current_level_name_translated : StringToUTF8(current_level_name_translated);
        presenseInfo.details = lname.c_str(); // название уровня
    }

    if (!lname.empty())
    {
        lname_and_task = lname;
        if (!task_txt.empty())
        {
            lname_and_task += " | ";
            lname_and_task += task_txt;
        }
    }
    else if (!task_txt.empty())
    {
        lname_and_task = task_txt;
    }

    if (!lname_and_task.empty())
        presenseInfo.largeImageText = lname_and_task.c_str(); // название уровня + активное задание на большой картинке

    presenseInfo.buttons[0] = {"AP-PRO", "https://ap-pro.ru/forums/topic/4427-der-fallende-stern-definitive-edition"};
    presenseInfo.buttons[1] = {"Discord", "https://discord.gg/rQ3XT8bDgs"};

    Discord_UpdatePresence(&presenseInfo);
#endif
}
