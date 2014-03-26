module Jekyll

  class MeiSupportedToLiquid < Liquid::Tag
    def initialize(tag_name, arg, tokens)
      super

      if arg.length == 0
        raise 'Please enter a yaml file path'
      else
        @yml_path = arg.strip!
      end
    end

    def render(context)
      doc = YAML::load(File.read( @yml_path ))
      
      doc["classes"].each do |class_|
        if class_["att-classes"]
          class_["att-classes"].each do |att_class|
            att_class_val = doc["att-classes"].find { |h| h['id'] == att_class }
            if att_class_val && att_class_val["attributes"]
              att_class_val["attributes"].each do |att_class_val_att|
                att_class_val_att[2] = att_class
                class_["attributes"] << att_class_val_att
              end
            end
          end
          class_["attributes"] = class_["attributes"].sort_by { |e| e[0] }
        end
        #puts class_.to_yaml
      end

      context.registers[:page]['doc'] = doc
      
      ""
    end
  end
end

Liquid::Template.register_tag('mei_supported_to_liquid', Jekyll::MeiSupportedToLiquid)