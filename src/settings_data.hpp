#pragma once
#include <fstream>


struct settings_data 
{
    bool best_minion_menu_compact = false;
    bool minion_menu_compact = false;
    bool ironman_mode = false;
    bool use_bazaar_enchanted_variants = false;
    int best_minion_display_amount = 0;

    void save(const std::string& path) const
    {
        nlohmann::json json_data = serialize();
        std::ofstream file(path);
        file << json_data.dump(4);
        file.close();
    }

    static settings_data load(const std::string& path)
    {
        nlohmann::json json_data;
        std::ifstream file(path);
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
        json_data["use_bazaar_enchanted_variants"] = use_bazaar_enchanted_variants;
        return json_data;
    }

    static settings_data deserialize(nlohmann::json json_data)
    {
        settings_data settings;
        settings.best_minion_menu_compact = json_data["best_minion_menu_compact"];
        settings.minion_menu_compact = json_data["minion_menu_compact"];
        settings.ironman_mode = json_data["ironman_mode"];
        settings.best_minion_display_amount = json_data["best_minion_display_amount"];
        settings.use_bazaar_enchanted_variants = json_data["use_bazaar_enchanted_variants"];
        return settings;
    }

   
};
