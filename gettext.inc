% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% reads translation files
%
% ( string -- array )
%
% string: name of a translation file
%
% array: translation array
%
% a translation file contains strings separated by '0' bytes:
%
% original1 0 translation1 0 original2 0 translation2 ...
%
% a translation array contains arrays of string pairs:
%  [
%    [original1 translation1]
%    [original2 translation2]
%    ...
%  ]
%
/read_translation_file {
  findfile
  dup 0 t_none settype eq {
    % file was not found
    pop
    /translations [ ] def
  } {
    /tmp_file_pointer exch def
    /tmp_state_reading_original    0 def
    /tmp_state_reading_translation 1 def
    /tmp_state tmp_state_reading_original def

    /translations [
      {
        tmp_file_pointer length 0 eq {
          % end of file
          tmp_state tmp_state_reading_translation eq {
            % close last translation pair
            ]
          } if
          exit
        } {
          % read a string
          [ 0 ] tmp_file_pointer read_string
          % jump over '0' byte
          1 add
          /tmp_file_pointer exch def
          tmp_state tmp_state_reading_original eq {
            % put new translation pair on stack
            [
            exch
            % change state
            /tmp_state tmp_state_reading_translation def
          } {
            % close translation pair
            ]
            % change state
            /tmp_state tmp_state_reading_original def
          } ifelse
        } ifelse
      } loop
    ] def
  } ifelse
} def

% - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
% translates a string
%
% ( string1 -- string2 )
%
% string1: original string
%
% string2: translated string
%
/gettext {
  translations {
    dup 0 get 2 index eq {
      % original string found
      % get translated string
      1 get
      dup "" eq {
        % untranslated string
        % leave original string on stack
        pop
      } {
        % remove original string from stack
        exch pop
      } ifelse
      return
    } {
      % this is another translation
      pop
    } ifelse
  } forall
} def