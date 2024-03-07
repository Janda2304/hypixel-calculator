#pragma once
#include <fstream>


struct settings_data 
{
    bool best_minion_menu_compact = false;
    bool minion_menu_compact = false;
    bool ironman_mode = false;
    int best_minion_display_amount = 0;

    void save() const
    {
        nlohmann::json json_data = serialize();
        std::ofstream file("../data/settings.json");
        file << json_data.dump(4);
        file.close();
    }

    static settings_data load() 
    {
        nlohmann::json json_data;
        std::ifstream file("../data/settings.json");
        file >> json_data;
        file.close();
        return deserialize(json_data);
    }
    

    nlohmann::json serialize() const 
    {
        nlohmann::json json_data;
        json_data["best_minion_menu_compact"] = best_minion_menu_compact;
        json_data["minion_menu_compact"] = minion_menu_compact;
        json_data["ironman_mode"] = ironman_mode;
        json_data["best_minion_display_amount"] = best_minion_display_amount;
        return json_data;
    }

    static settings_data deserialize(nlohmann::json json_data)
    {
        settings_data settings;
        settings.best_minion_menu_compact = json_data["best_minion_menu_compact"];
        settings.minion_menu_compact = json_data["minion_menu_compact"];
        settings.ironman_mode = json_data["ironman_mode"];
        settings.best_minion_display_amount = json_data["best_minion_display_amount"];
        return settings;
    }

   
};
