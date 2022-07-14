script_name("Ateyism")
script_author("THERION")

local encoding = require("encoding")
encoding.default = "CP1251"
local u8 = encoding.UTF8

local events = require("samp.events")

local dictionary = {}

local function create_dictionary_unit(pattern, substitute)
   return {
      pattern = u8:decode(pattern),
      substitute = u8:decode(substitute),
   }
end

local function ateyism_handler(dictionary, text)
   local result = ""
   for word in string.gmatch(text, "%S+") do
      for _, unit in ipairs(dictionary) do
         local pattern = unit.pattern
         local substitute = unit.substitute
      end
   end
   -- TODO: реалізувати заміну слова
   return u8:decode("бэбра")
end

function events.onServerMessage(color, text)
   local ateyistic_text = ateyism_handler(dictionary, text)
   return { color, ateyistic_text }
end

function events.onChatMessage(playerId, text)
   local ateyistic_text = ateyism_handler(dictionary, text)
   return { playerId, text }
end

function events.onShowDialog(dialogId, style, title, button1, button2, text)
   local ateyistic_title = ateyism_handler(dictionary, title)
   local ateyistic_button1 = ateyism_handler(dictionary, button1)
   local ateyistic_button2 = ateyism_handler(dictionary, button2)
   local ateyistic_text = ateyism_handler(dictionary, text)
   return { dialogId, style, ateyistic_title, ateyistic_button1, ateyistic_button2, ateyistic_text }
end

-- додати хук на нікнейм
-- додати хук на chat bubble
-- додати хук на введення до чату
-- додати хук на введення до діалогу

dictionary = {
   create_dictionary_unit("боге", "науке"),
   create_dictionary_unit("богом", "наукой"),
   create_dictionary_unit("богу", "науке"),
   create_dictionary_unit("бога", "науки"),
   create_dictionary_unit("боже", "наука"),
   -- однина без "бог"
   create_dictionary_unit("богах", "науках"),
   create_dictionary_unit("богами", "науками"),
   create_dictionary_unit("богов", "наук"),
   create_dictionary_unit("богам", "наукам"),
   create_dictionary_unit("богов", "наук"),
   create_dictionary_unit("боги", "науки"),
   -- множина
   create_dictionary_unit("бог", "наука"),
   --

   -- TODO: додати інші слова
}