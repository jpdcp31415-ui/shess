use colored::CustomColor as Colour;

#[derive(Clone)]
pub enum SettingType {
    Bool(bool),
    Int(i32),
    Char(char),
    Str(&'static str),
    Float(f32),
    Colour(Colour),
}

pub struct SettingData {
    curr: SettingType,
    def: SettingType,
}

impl SettingData {
    pub fn new(data: SettingType) -> Self {
        Self {
            curr: data.clone(),
            def: data.clone(),
        }
    }

    pub fn get_curr(&self) -> SettingType {
        self.curr.clone()
    }

    pub fn get_def(&self) -> SettingType {
        self.def.clone()
    }

    pub fn set_curr(&mut self, new_data: SettingType) {
        self.curr = new_data.clone();
    }
}

pub struct Setting {
    pub name: &'static str,
    pub data: SettingData,
    pub is_valid_value: Option<fn(&SettingType) -> &'static str>,
}

impl Setting {
    pub fn find<'a>(settings: &'a [Setting], name: &'static str) -> Option<&'a Setting> {
        for i in 0..(settings.len()) {
            if settings[i].name == name {
                return Some(&settings[i]);
            }
        }
        None
    }
}
